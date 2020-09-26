-- Faça uma função capaz de aplicar um aumento de 10% em todos os funcionários.
create or replace function aumentoFunc () returns boolean as $$
declare

	emp        record;
    vlrAumento float;

BEGIN
	
    for emp in select * from employee
    loop

    	vlrAumento := emp.salary + (emp.salary * 0.1);

    	update employee e
    	   set salary   = vlrAumento
    	 where id       = emp.id;

   	end loop;

   	return true;

END;
$$ LANGUAGE plpgsql;