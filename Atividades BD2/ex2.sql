-- Faça uma função capaz de aplicar um aumento de X% nos funcionários com id maior que N. X e N serão passados por argumento.
create or replace function aumentoFunc2 (aumento float, idMin int) returns boolean as $$
declare

	emp        record;
    vlrAumento float;

BEGIN
	
    for emp in select * from employee
    loop

    	vlrAumento := emp.salary + (emp.salary * (aumento/100));

    	if emp.id > idMin then

	    	update employee e
	    	   set salary   = vlrAumento
	    	 where id       = emp.id;

	    end if;

   	end loop;

   	return true;

END;
$$ LANGUAGE plpgsql;