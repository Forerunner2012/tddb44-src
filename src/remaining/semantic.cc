#include "semantic.hh"


semantic *type_checker = new semantic();


/* Used to check that all functions contain return statements.
   Static means that it is only visible inside this file.
   It is set to false in do_typecheck() (ie, every time we start type checking
   a new block) and set to true if we find an ast_return node. See below. */
static bool has_return = false;


/* Interface for type checking a block of code represented as an AST node. */
void semantic::do_typecheck(symbol *env, ast_stmt_list *body)
{
    // Reset the variable, since we're checking a new block of code.
    has_return = false;
    if (body) {
        body->type_check();
    }

    // This is the only case we need this variable for - a function lacking
    // a return statement. All other cases are already handled in
    // ast_return::type_check(); see below.
    if (env->tag == SYM_FUNC && !has_return) {
        // Note: We could do this by overloading the do_typecheck() method -
        // one for ast_procedurehead and one for ast_functionhead, but this
        // will do... Hopefully people won't write empty functions often,
        // since in that case we won't have position information available.
        if (body != NULL) {
            type_error(body->pos) << "A function must return a value.\n";
        } else {
            type_error() << "A function must return a value.\n";
        }
    }
}


/* Compare formal vs. actual parameters recursively. */
bool semantic::chk_param(ast_id *env,
                        parameter_symbol *formals,
                        ast_expr_list *actuals)
{
    /* Your code here */
	//3 cases to manage for formals and actuals (check type of last_expr)
	//1- two same types except NULL (can't return true: need to inspect the precedings)
	//2- only one NULL (false)
	//3- both NULL (true)
	if (formals == NULL && actuals == NULL){
		return true;
	}
	if (formals == NULL || actuals == NULL){
		return false;
	}
	if(formals->type == actuals->last_expr->type){  
      return chk_param(env,formals->preceding,actuals->preceding);
    }
	return false;
}


/* Check formal vs. actual parameters at procedure/function calls. */
void semantic::check_parameters(ast_id *call_id,
                                ast_expr_list *param_list)
{
    /* Your code here */
	//Check type
	if(param_list != NULL) {
		param_list->type_check();
	}

	symbol* s = sym_tab->get_symbol(call_id->sym_p);

	//Separate operations with function and procedure
	if (s->tag == SYM_FUNC){
		//Get the specific function symbol and check its last_parameter recursively
		function_symbol* func_s = s->get_function_symbol(); 
		parameter_symbol* formals = func_s->last_parameter;

		if(!chk_param(call_id, formals, param_list)) {
			type_error(call_id->pos) << "Error with the argument type in the function call (check_parameters)"
				<< endl;
		}
	}
	else if (s->tag == SYM_PROC){
		//Get the specific procedure symbol and check its last_parameter recursively
		procedure_symbol* proc_s = s->get_procedure_symbol(); 
		parameter_symbol* formals = proc_s->last_parameter;

		////Check if the param_list is same type
		if(!chk_param(call_id, formals, param_list)) {
			type_error(call_id->pos) << "argument type error in procedure call" << endl;
		}
	}
	else{
		cout << "Neither a function or a procedure (check_parameters)" << endl;
	}
}



/* We overload this method for the various ast_node subclasses that can
   appear in the AST. By use of virtual (dynamic) methods, we ensure that
   the correct method is invoked even if the pointers in the AST refer to
   one of the abstract classes such as ast_expression or ast_statement. */
sym_index ast_node::type_check()
{
    fatal("Trying to type check abstract class ast_node.");
    return void_type;
}

sym_index ast_statement::type_check()
{
    fatal("Trying to type check abstract class ast_statement.");
    return void_type;
}

sym_index ast_expression::type_check()
{
    fatal("Trying to type check abstract class ast_expression.");
    return void_type;
}

sym_index ast_lvalue::type_check()
{
    fatal("Trying to type check abstract class ast_lvalue.");
    return void_type;
}

sym_index ast_binaryoperation::type_check()
{
    fatal("Trying to type check abstract class ast_binaryoperation.");
    return void_type;
}

sym_index ast_binaryrelation::type_check()
{
    fatal("Trying to type check abstract class ast_binaryrelation.");
    return void_type;
}



/* Type check a list of statements. */
sym_index ast_stmt_list::type_check()
{
    if (preceding != NULL) {
        preceding->type_check();
    }
    if (last_stmt != NULL) {
        last_stmt->type_check();
    }
    return void_type;
}


/* Type check a list of expressions. */
sym_index ast_expr_list::type_check()
{
    /* Your code here */
	//Same as above for expr
    if (preceding != NULL) {
        preceding->type_check();
    }
    if (last_expr != NULL) {
        last_expr->type_check();
    }
    return void_type;
}



/* Type check an elsif list. */
sym_index ast_elsif_list::type_check()
{
    /* Your code here */
	//Same as above for elsif
    if (preceding != NULL) {
        preceding->type_check();
    }
    if (last_elsif != NULL) {
        last_elsif->type_check();
    }
    return void_type;
}


/* "type check" an indentifier. We need to separate nametypes from other types
   here, since all nametypes are of type void, but should return an index to
   itself in the symbol table as far as typechecking is concerned. */
sym_index ast_id::type_check()
{
    if (sym_tab->get_symbol(sym_p)->tag != SYM_NAMETYPE) {
        return type;
    }
    return sym_p;
}


sym_index ast_indexed::type_check()
{
    /* Your code here */
	//Check if the type of index in the array is only integers
    if (index->type_check() != integer_type) {
		//Should write in pos alone: use struct index
        type_error(index->pos) << "Array indexes can only be integers (ast_indexed)" << endl;
    }
	//return the type (shall be integer)
	type = id->type_check();
    return type;
}



/* This convenience function is used to type check all binary operations
   in which implicit casting of integer to real is done: plus, minus,
   multiplication. We synthesize type information as well. */
sym_index semantic::check_binop1(ast_binaryoperation *node)
{
    /* Your code here */
    // You don't have to use this method but it might be convenient
	//However it acts as a interface for type_check functins from different ast types
	//Used for add, sub, mul, divide
	//Cases to manage:
	//1- some are void_type		=> return error
	//2- same type				=> return their common type
	//3- different and not void => check if integer on left or real (apply 8.1 p.96)
	//Only need to manage integer and real ("DIESEL is rather simple")
		
	//Get the types of right and left values
	sym_index left_type = node->left->type_check();
	sym_index right_type = node->right->type_check();

	if (right_type == void_type || left_type == void_type){
		//1-
		type_error(node->pos) << "Detect not allowed void_type in the expr (check_binop1)" << endl; 
		return void_type;
	}
	else if (right_type == left_type) {
		//2-
		node->type = left_type;
		return left_type;
	}
	else {
		//3-
		if (left_type == integer_type) {
			//create a ast_cast node between the node and its left node
			node->left = new ast_cast(node->left->pos, node->left);
		}
		else {
			//create a ast_cast node between the node and its right node
			node->right = new ast_cast(node->right->pos, node->right);
		}
        node->type = real_type;
        return real_type;
	}
}

sym_index ast_add::type_check()
{
    /* Your code here */
    return type_checker->check_binop1(this);
}

sym_index ast_sub::type_check()
{
    /* Your code here */
    return type_checker->check_binop1(this);
}

sym_index ast_mult::type_check()
{
    /* Your code here */
    return type_checker->check_binop1(this);
}

/* Divide is a special case, since it always returns real. We make sure the
   operands are cast to real too as needed. */
sym_index ast_divide::type_check()
{
    /* Your code here */
	//Detect errors and convert all integer into real

	//Get the types of right and left values
	// ! Do not need to put node-> because we actually work on the object itself
	sym_index left_type = left->type_check();
	sym_index right_type = right->type_check();

	if (right_type == void_type || left_type == void_type){
		//1-
		type_error(pos) << "Detect not allowed void_type in the divide expr (ast_divide::type_check)"
			<< endl; 
		return void_type;
	}
	if (right_type == integer_type) {
		right = new ast_cast(right->pos, right);
	}
	if (left_type == integer_type) {
		left = new ast_cast(left->pos, left);
	}
	type = real_type;
    return real_type;
}



/* This convenience method is used to type check all binary operations
   which only accept integer operands: AND, OR, MOD, DIV.
   The second argument is the name of the operator, so we can generate a
   good error message.
   All of these return integers, so we synthesize that.
   */
sym_index semantic::check_binop2(ast_binaryoperation *node, string s)
{
    /* Your code here */
	//Used for and, or, mod, div (not divide)
	//Only accept integer in left and right nodes
    if(node->left->type_check() != integer_type){
        type_error(node->left->pos) << "Left value of operation " << s << " must be an integer" << endl;
    }
    if(node->right->type_check() != integer_type){
        type_error(node->left->pos) << "Right value of operation " << s << " must be an integer" << endl;
    }
	node->type = integer_type;
	return integer_type;
}

sym_index ast_or::type_check()
{
    /* Your code here */
	return type_checker->check_binop2(this, "OR");
}

sym_index ast_and::type_check()
{
    /* Your code here */
    return type_checker->check_binop2(this, "AND");
}

sym_index ast_idiv::type_check()
{
    /* Your code here */
    return type_checker->check_binop2(this, "DIV");
}

sym_index ast_mod::type_check()
{
    /* Your code here */
    return type_checker->check_binop2(this, "MOD");
}



/* Convienience method for all binary relations, since they're all typechecked
   the same way. They all return integer types, 1 = true, 0 = false. */
sym_index semantic::check_binrel(ast_binaryrelation *node)
{
    /* Your code here */
	//Really similar as before (binop1 & 2)

	//Get the types of right and left values
	sym_index left_type = node->left->type_check();
	sym_index right_type = node->right->type_check();

	if (right_type == void_type || left_type == void_type){
		//1-
		type_error(node->pos) << "Detect not allowed void_type in a binary condition (check_binrel)" << endl; 
		return void_type;
	}
	else if (right_type == left_type) {
		//2-
		node->type = integer_type;
		return integer_type;
	}
	else {
		//3-
		if (left_type == integer_type) {
			//create a ast_cast node between the node and its left node
			node->left = new ast_cast(node->left->pos, node->left);
		}
		else {
			//create a ast_cast node between the node and its right node
			node->right = new ast_cast(node->right->pos, node->right);
		}
		node->type = integer_type;
		return integer_type;
	}
}

sym_index ast_equal::type_check()
{
    /* Your code here */
    return type_checker->check_binrel(this);
}

sym_index ast_notequal::type_check()
{
    /* Your code here */
    return type_checker->check_binrel(this);
}

sym_index ast_lessthan::type_check()
{
    /* Your code here */
    return type_checker->check_binrel(this);
}

sym_index ast_greaterthan::type_check()
{
    /* Your code here */
    return type_checker->check_binrel(this);
}



/*** The various classes derived from ast_statement. ***/

sym_index ast_procedurecall::type_check()
{
    /* Your code here */
    type_checker->check_parameters(id, parameter_list);
    return void_type;
}


sym_index ast_assign::type_check()
{
    /* Your code here */
	//Get the types of right and left values. From ast.hh:
    // The left hand side (lhs), ie, the variable being assigned to.
    //ast_lvalue *lhs;
    // The right hand side (rhs), ie, the value being assigned.
    //ast_expression *rhs;
	sym_index left_type_tmp = lhs->type_check();
	sym_index right_type_tmp = rhs->type_check();

	//Cases to manage
	//(int, real) , (void_type somewhere) , (real, integer)
	if (left_type_tmp == integer_type && right_type_tmp == real_type) {
		type_error(pos) << "Can't assign a real to an integer (ast_assign::type_check)" << endl;
	}
	else if (left_type_tmp == void_type || right_type_tmp == void_type) {
		type_error(pos) << "Can't assign a void type (ast_assign::type_check)" << endl;
	}
	else if (left_type_tmp == real_type && right_type_tmp == integer_type) {
		rhs = new ast_cast(rhs->pos, rhs);
	}
	return left_type_tmp;
	}


sym_index ast_while::type_check()
{
    if (condition->type_check() != integer_type) {
        type_error(condition->pos) << "while predicate must be of integer "
                                   << "type.\n";
    }

    if (body != NULL) {
        body->type_check();
    }
    return void_type;
}


sym_index ast_if::type_check()
{
    /* Your code here */
	//Quite similar to previous while with condition type check then bodies
	if (condition->type_check() != integer_type) {
        type_error(condition->pos) << "If condition must be of integer type.\n";
    }
	body->type_check();
    if(elsif_list != NULL) {
        elsif_list->type_check();
    }
    if(else_body != NULL) {
        else_body->type_check();
    }
    return void_type;
}


sym_index ast_return::type_check()
{
    // This static global (meaning it is global for all methods in this file,
    // but not visible outside this file) variable is reset to 0 every time
    // we start type checking a new block of code. If we find a return
    // statement, we set it to 1. It is checked in the do_typecheck() method
    // of semantic.cc.
    has_return = true;

    // Get the current environment. We don't yet know if it's a procedure or
    // a function.
    symbol *tmp = sym_tab->get_symbol(sym_tab->current_environment());
    if (value == NULL) {
        // If the return value is NULL,
        if (tmp->tag != SYM_PROC)
            // ...and we're not inside a procedure, something is wrong.
        {
            type_error(pos) << "Must return a value from a function.\n";
        }
        return void_type;
    }

    sym_index value_type = value->type_check();

    // The return value is not NULL,
    if (tmp->tag != SYM_FUNC) {
        // ...so if we're not inside a function, something is wrong too.
        type_error(pos) << "Procedures may not return a value.\n";
        return void_type;
    }

    // Now we know it's a function and can safely downcast.
    function_symbol *func = tmp->get_function_symbol();

    // Must make sure that the return type matches the function's
    // declared return type.
    if (func->type != value_type) {
        type_error(value->pos) << "Bad return type from function.\n";
    }

    return void_type;
}


sym_index ast_functioncall::type_check()
{
    /* Your code here */
	//Check if the parameters are type-correct and check if the function is not void
	//As before, we got direct access to the object attributes

/*Changed to return type via a get function
type_checker->check_parameters(id, parameter_list);
    if (type == void_type)
        type_error(pos) << "Functions can't return void. A value need to be return." << endl;
    return type;
*/

	type_checker->check_parameters(id, parameter_list);
	symbol* sym = sym_tab->get_symbol(id->sym_p);
	return sym->type;
}

sym_index ast_uminus::type_check()
{
    /* Your code here */
	if(expr->type_check() != integer_type && expr->type_check() != real_type)
		type_error(pos) << "Uminus operand must be integer_type or real_type (ast_uminus).\n";
	type= expr->type_check();
    return type;
}

sym_index ast_not::type_check()
{
    /* Your code here */
	if(expr->type_check() != integer_type) {
		type_error(pos) << "Not operand type must be integer. (ast_not::type_check)" << endl;
	}
	return integer_type;
}


sym_index ast_elsif::type_check()
{
    /* Your code here */
	//Quite similar to if with less bodies (1 condition + 1 body)  
	if (condition->type_check() != integer_type) {
        type_error(condition->pos) << "ElseIf condition type must be integer (ast_elsif::type_check)" << endl;
    }
	if(body != NULL) {
		body->type_check();
	}
    return void_type;
}



sym_index ast_integer::type_check()
{
    return integer_type;
}

sym_index ast_real::type_check()
{
    return real_type;
}
