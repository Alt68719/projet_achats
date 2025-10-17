-- Conversion de MySQL à SQLite

-- Création des tables

CREATE TABLE IF NOT EXISTS entrepot (
  ref_entrepot VARCHAR(20) NOT NULL,
  adresse VARCHAR(200) NOT NULL,
  PRIMARY KEY (ref_entrepot)
);

---

CREATE TABLE IF NOT EXISTS fournisseur (
  ref_frs VARCHAR(20) NOT NULL,
  raison_sociale VARCHAR(50) NOT NULL,
  NIF INTEGER NOT NULL,
  adresse VARCHAR(200) NOT NULL,
  tel_frs INTEGER NOT NULL,
  PRIMARY KEY (ref_frs),
  UNIQUE (NIF) -- UNIQUE INDEX `NIF_UNIQUE` est converti en contrainte UNIQUE
);

---

CREATE TABLE IF NOT EXISTS marchandise (
  ref_mar VARCHAR(20) NOT NULL,
  design VARCHAR(100) NOT NULL,
  prix_de_vente REAL NOT NULL, -- DECIMAL(10,2) devient REAL en SQLite pour les nombres décimaux
  PRIMARY KEY (ref_mar)
);

---

CREATE TABLE IF NOT EXISTS livrer (
  num_livraison VARCHAR(20) NOT NULL,
  ref_frs VARCHAR(20) NOT NULL,
  date_de_livraison DATE NOT NULL,
  prix_achat REAL NOT NULL, -- DECIMAL(10,2) devient REAL
  qte_livre INTEGER NOT NULL, -- INT devient INTEGER
  ref_mar VARCHAR(20) NOT NULL,
  ref_entrepot VARCHAR(20) NOT NULL,
  PRIMARY KEY (num_livraison),
  -- Clés étrangères
  FOREIGN KEY (ref_entrepot)
    REFERENCES entrepot (ref_entrepot)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  FOREIGN KEY (ref_frs)
    REFERENCES fournisseur (ref_frs)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  FOREIGN KEY (ref_mar)
    REFERENCES marchandise (ref_mar)
    ON DELETE CASCADE
    ON UPDATE CASCADE
);

-- SQLite ne nécessite pas d'INDEX explicite pour les colonnes de clé étrangère
-- mais ils sont maintenus pour des raisons de performance.

CREATE INDEX IF NOT EXISTS idx_livrer_ref_frs ON livrer (ref_frs);
CREATE INDEX IF NOT EXISTS idx_livrer_ref_mar ON livrer (ref_mar);
CREATE INDEX IF NOT EXISTS idx_livrer_ref_entrepot ON livrer (ref_entrepot);

---

CREATE TABLE IF NOT EXISTS stocker (
  num_operat VARCHAR(20) NOT NULL,
  ref_mar VARCHAR(20) NOT NULL,
  ref_entrepot VARCHAR(20) NOT NULL,
  qte_stock INTEGER DEFAULT 0, -- INT devient INTEGER
  PRIMARY KEY (num_operat),
  -- Clés étrangères
  FOREIGN KEY (ref_entrepot)
    REFERENCES entrepot (ref_entrepot)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
  FOREIGN KEY (ref_mar)
    REFERENCES marchandise (ref_mar)
    ON DELETE CASCADE
    ON UPDATE CASCADE
);

CREATE INDEX IF NOT EXISTS idx_stocker_ref_mar ON stocker (ref_mar);
CREATE INDEX IF NOT EXISTS idx_stocker_ref_entrepot ON stocker (ref_entrepot);

---

-- Création des vues

CREATE VIEW IF NOT EXISTS bon_livraison AS
SELECT
  l.num_livraison AS Num_Livraison,
  l.date_de_livraison AS Date_de_Livraison,
  f.raison_sociale AS Fournisseur,
  f.adresse AS AdresseFournisseur,
  f.tel_frs AS TelFournisseur,
  m.design AS Marchandise,
  l.qte_livre AS Qte_Livre,
  l.prix_achat AS Prix_Achat,
  (l.qte_livre * l.prix_achat) AS MontantTotal
FROM livrer AS l
LEFT JOIN fournisseur AS f ON l.ref_frs = f.ref_frs
LEFT JOIN marchandise AS m ON l.ref_mar = m.ref_mar;

---

-- Note : La vue `inventaire` originale en MySQL contenait des conditions de jointure
-- potentiellement incorrectes (`m.ref_mar = m.ref_mar` et `e.ref_entrepot = e.ref_entrepot`).
-- J'ai supposé que le but était de joindre `stocker` (s) avec `marchandise` (m), `entrepot` (e)
-- et d'utiliser une livraison pour la date, bien que la jointure sur `livrer` semble peu claire
-- sans condition de liaison entre `stocker` et `livrer`.
-- J'ai conservé la logique de jointure telle quelle, en corrigeant les conditions évidentes,
-- mais vous pourriez vouloir revoir la logique pour `livrer` (l).

CREATE VIEW IF NOT EXISTS inventaire AS
SELECT
  s.ref_mar AS Ref_Mar,
  m.design AS Design,
  s.ref_entrepot AS Ref_Entrepot,
  e.adresse AS Entrepot,
  l.date_de_livraison AS date_de_Livraison, -- La date de livraison n'est pas directement liée au stock.
  s.qte_stock AS Qte_Stock
FROM stocker AS s
JOIN marchandise AS m ON s.ref_mar = m.ref_mar -- Correction: m.ref_mar = m.ref_mar -> s.ref_mar = m.ref_mar
JOIN entrepot AS e ON s.ref_entrepot = e.ref_entrepot -- Correction: e.ref_entrepot = e.ref_entrepot -> s.ref_entrepot = e.ref_entrepot
JOIN livrer AS l ON s.num_operat = l.num_livraison; -- La jointure l.num_livraison = l.num_livraison a été remplacée par une jointure spéculative.

---

CREATE VIEW IF NOT EXISTS prix_max AS
SELECT
  m.ref_mar AS Ref_Mar,
  m.design AS Design,
  SUM(l.qte_livre * l.prix_achat) AS Prix_Total
FROM livrer AS l
JOIN marchandise AS m ON l.ref_mar = m.ref_mar
WHERE l.date_de_livraison = '2025-05-04'
GROUP BY m.ref_mar, m.design;

---

-- Création du déclencheur (TRIGGER)

-- En SQLite, `CONCAT` n'est pas une fonction standard. J'utilise l'opérateur de concaténation `||`.
-- J'ai également retiré les délimiteurs (`DELIMITER $$...$$`) spécifiques à MySQL.

CREATE TRIGGER IF NOT EXISTS maj_stock
AFTER INSERT ON livrer
FOR EACH ROW
BEGIN
  SELECT CASE
    -- Si la ligne existe dans `stocker`, on la met à jour
    WHEN EXISTS (SELECT 1 FROM stocker
                 WHERE ref_mar = NEW.ref_mar AND ref_entrepot = NEW.ref_entrepot) THEN
      UPDATE stocker
      SET qte_stock = qte_stock + NEW.qte_livre
      WHERE ref_mar = NEW.ref_mar AND ref_entrepot = NEW.ref_entrepot;

    -- Sinon, on insère une nouvelle ligne
    ELSE
      INSERT INTO stocker (num_operat, ref_mar, ref_entrepot, qte_stock)
      VALUES ('OP' || NEW.num_livraison, NEW.ref_mar, NEW.ref_entrepot, NEW.qte_livre);
  END;
END;