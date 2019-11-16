CREATE OR REPLACE FUNCTION armazenamentolugares() RETURNS trigger AS $$
DECLARE 
    precoPassagem FLOAT;
    poltronasDisponiveis INTEGER;
    BEGIN      
        --QUANTIDADE DE ASSENTOS
        poltronasDisponiveis = (SELECT numberOfFreeSeats FROM AvailableFlights WHERE num_flight = NEW.num_flight); 
        
        --VERIFICAR SE EXISTEM ASSENTOS DISPONIVEIS PARA O VO
        IF poltronasDisponiveis <= 0 THEN 
            RAISE EXCEPTION 'Nao ha poltronas disponiveis';
        END IF;
    
        IF poltronasDisponiveis IS NULL THEN 
            RAISE EXCEPTION 'Nao ha voo disponivel com este numero';
        END IF;
    
        --PEGAR O PREÇO DA PASSAGEM
        precoPassagem := (SELECT price FROM AvailableFlights WHERE num_flight = NEW.num_flight);
   
        --ATUALIZAR A TABELA DE VOOS DISPONIVEIS
        --DIMINUIR A QUANTIDADE DE POLTRONAS VAZIAS
        --AUMENTAR O PRECO DA PASSAGEM EM 50 
        UPDATE AvailableFlights
           SET numberoffreeseats = numberoffreeseats - 1,
               price = precoPassagem + 50 
         WHERE num_flight = NEW.num_flight;
        
        RETURN NEW;
    END;
$$ LANGUAGE plpgsql;

-------------------------------------------------------------------------------------

CREATE TRIGGER armazenamentolugares BEFORE INSERT ON
public.bookings FOR EACH ROW EXECUTE PROCEDURE armazenamentolugares();