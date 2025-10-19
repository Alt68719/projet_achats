CREATE TABLE IF NOT EXISTS entrepot (
  ref_entrepot VARCHAR(20) NOT NULL,
  adresse VARCHAR(200) NOT NULL,
  PRIMARY KEY (ref_entrepot)
);


CREATE TABLE IF NOT EXISTS fournisseur (
  ref_frs VARCHAR(20) NOT NULL,
  raison_sociale VARCHAR(50) NOT NULL,
  NIF INTEGER NOT NULL,
  adresse VARCHAR(200) NOT NULL,
  tel_frs INTEGER NOT NULL,
  PRIMARY KEY (ref_frs),
  UNIQUE (NIF)
);


CREATE TABLE IF NOT EXISTS marchandise (
  ref_mar VARCHAR(20) NOT NULL,
  design VARCHAR(100) NOT NULL,
  prix_de_vente REAL NOT NULL,
  PRIMARY KEY (ref_mar)
);


CREATE TABLE IF NOT EXISTS livrer (
  num_livraison VARCHAR(20) NOT NULL,
  ref_frs VARCHAR(20) NOT NULL,
  date_de_livraison DATE NOT NULL,
  prix_achat REAL NOT NULL,
  qte_livre INTEGER NOT NULL,
  ref_mar VARCHAR(20) NOT NULL,
  ref_entrepot VARCHAR(20) NOT NULL,
  PRIMARY KEY (num_livraison),
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


CREATE INDEX IF NOT EXISTS idx_livrer_ref_frs ON livrer (ref_frs);
CREATE INDEX IF NOT EXISTS idx_livrer_ref_mar ON livrer (ref_mar);
CREATE INDEX IF NOT EXISTS idx_livrer_ref_entrepot ON livrer (ref_entrepot);

CREATE TABLE IF NOT EXISTS stocker (
  num_operat VARCHAR(20) NOT NULL,
  ref_mar VARCHAR(20) NOT NULL,
  ref_entrepot VARCHAR(20) NOT NULL,
  qte_stock INTEGER DEFAULT 0,
  PRIMARY KEY (num_operat),

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


CREATE VIEW IF NOT EXISTS inventaire AS
SELECT
  s.ref_mar AS Ref_Mar,
  m.design AS Design,
  s.ref_entrepot AS Ref_Entrepot,
  e.adresse AS Entrepot,
  l.date_de_livraison AS date_de_Livraison,
  s.qte_stock AS Qte_Stock
FROM stocker AS s
JOIN marchandise AS m ON s.ref_mar = m.ref_mar
JOIN entrepot AS e ON s.ref_entrepot = e.ref_entrepot
JOIN livrer AS l ON s.num_operat = l.num_livraison;

CREATE VIEW IF NOT EXISTS prix_max AS
SELECT
  m.ref_mar AS Ref_Mar,
  m.design AS Design,
  SUM(l.qte_livre * l.prix_achat) AS Prix_Total
FROM livrer AS l
JOIN marchandise AS m ON l.ref_mar = m.ref_mar
WHERE l.date_de_livraison = '2025-05-04'
GROUP BY m.ref_mar, m.design;



CREATE TRIGGER IF NOT EXISTS maj_stock
AFTER INSERT ON livrer
FOR EACH ROW
BEGIN
  SELECT CASE
    WHEN EXISTS (SELECT 1 FROM stocker
                 WHERE ref_mar = NEW.ref_mar AND ref_entrepot = NEW.ref_entrepot) THEN
      UPDATE stocker
      SET qte_stock = qte_stock + NEW.qte_livre
      WHERE ref_mar = NEW.ref_mar AND ref_entrepot = NEW.ref_entrepot;

    ELSE
      INSERT INTO stocker (num_operat, ref_mar, ref_entrepot, qte_stock)
      VALUES ('OP' || NEW.num_livraison, NEW.ref_mar, NEW.ref_entrepot, NEW.qte_livre);
  END;
END;
