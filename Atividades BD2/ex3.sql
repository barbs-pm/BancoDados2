-- Faça uma função capaz de remover os funcionar com salário acima da média.
create or replace function deletaFunc () returns boolean as $$
declare

	emp        record;
    avgSalario float;

BEGIN
	
	select avg(salary)
	  into avgSalario
	  from employee;
	

    for emp in select * from employee
    loop

    	if emp.salary > avgSalario then

	    	delete 
	          from employee
	    	 where id = emp.id;

	    end if;

   	end loop;

   	return true;

END;
$$ LANGUAGE plpgsql;