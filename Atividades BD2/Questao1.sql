CREATE OR REPLACE FUNCTION adicionaAuditoria() RETURNS TRIGGER AS $$
    BEGIN
        IF (TG_OP = 'DELETE') THEN
            INSERT INTO auditoria (empregado_id,cpf,num_departamento,salario,supervisor,evento,usuario,dia)
            VALUES (OLD.id,OLD.cpf,OLD.num_departamento,OLD.salario,OLD.supervisor,1,current_user,current_timestamp);
            RETURN OLD;
        ELSIF (TG_OP = 'UPDATE') THEN
            INSERT INTO auditoria (empregado_id,cpf,num_departamento,salario,supervisor,evento,usuario,dia)
            VALUES (NEW.id,NEW.cpf,NEW.num_departamento,NEW.salario,NEW.supervisor,2,current_user,current_timestamp);
            RETURN NEW;
        ELSIF (TG_OP = 'INSERT') THEN
            INSERT INTO auditoria (empregado_id,cpf,num_departamento,salario,supervisor,evento,usuario,dia)
            VALUES (NEW.id,NEW.cpf,NEW.num_departamento,NEW.salario,NEW.supervisor,3,current_user,current_timestamp);
            RETURN NEW;
        END IF;
        RETURN NULL; 
    END;
$$ LANGUAGE plpgsql;

----------------------------------------------------------------------------

CREATE TRIGGER armazenamentolugares AFTER INSERT OR UPDATE OR DELETE ON
empregado FOR EACH ROW EXECUTE PROCEDURE adicionaAuditoria();