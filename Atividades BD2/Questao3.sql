CREATE OR REPLACE FUNCTION adicionaAuditoriaWiki() RETURNS TRIGGER AS $$
DECLARE
    ultimaRevisao INTEGER;
    tamanhoText INTEGER;
    BEGIN
        --O NOME SERA TRATADO COMO CHAVE PRIMARIA PARA DIFERENCIAR OS ARTIGOS DA WIKI
        
        IF (TG_OP = 'INSERT') THEN
            ultimaRevisao = 1;
        ELSIF (TG_OP = 'UPDATE') THEN
            ultimaRevisao = (SELECT count(*) FROM pagerevision WHERE nome = NEW.nome) + 1;
        END IF;    
    
	tamanhoText = CHAR_LENGTH(NEW.text) - CHAR_LENGTH(OLD.text);

        INSERT INTO pageaudit
        VALUES(NEW.nome, NEW.dia, tamanhoText, ultimaRevisao, current_timestamp, NEW.texto, current_user);
    
        RETURN NEW; 
    END;
$$ LANGUAGE plpgsql;

----------------------------------------------------------------------------

CREATE TRIGGER armazenamentolugares AFTER INSERT OR UPDATE ON
PageRevision FOR EACH ROW EXECUTE PROCEDURE adicionaAuditoriaWiki();
