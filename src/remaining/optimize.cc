#include "optimize.hh"

/*** This file contains all code pertaining to AST optimisation. It currently
     implements a simple optimisation called "constant folding". Most of the
     methods in this file are empty, or just relay optimize calls downward
     in the AST. If a more powerful AST optimization scheme were to be
     implemented, only methods in this file should need to be changed. ***/


ast_optimizer *optimizer = new ast_optimizer();


/* The optimizer's interface method. Starts a recursive optimize call down
   the AST nodes, searching for binary operators with constant children. */
void ast_optimizer::do_optimize(ast_stmt_list *body)
{
    if (body != NULL) {
        body->optimize();
    }
}


/* Returns 1 if an AST expression is a subclass of ast_binaryoperation,
   ie, eligible for constant folding. */
bool ast_optimizer::is_binop(ast_expression *node)
{
    switch (node->tag) {
    case AST_ADD:
    case AST_SUB:
    case AST_OR:
    case AST_AND:
    case AST_MULT:
    case AST_DIVIDE:
    case AST_IDIV:
    case AST_MOD:
        return true;
    default:
        return false;
    }
}

/* We overload this method for the various ast_node subclasses that can
   appear in the AST. By use of virtual (dynamic) methods, we ensure that
   the correct method is invoked even if the pointers in the AST refer to
   one of the abstract classes such as ast_expression or ast_statement. */
void ast_node::optimize()
{
    fatal("Trying to optimize abstract class ast_node.");
}

void ast_statement::optimize()
{
    fatal("Trying to optimize abstract class ast_statement.");
}

void ast_expression::optimize()
{
    fatal("Trying to optimize abstract class ast_expression.");
}

void ast_lvalue::optimize()
{
    fatal("Trying to optimize abstract class ast_lvalue.");
}

void ast_binaryoperation::optimize()
{
    fatal("Trying to optimize abstract class ast_binaryoperation.");
}

void ast_binaryrelation::optimize()
{
    fatal("Trying to optimize abstract class ast_binaryrelation.");
}



/*** The optimize methods for the concrete AST classes. ***/

/* Optimize a statement list. */
void ast_stmt_list::optimize()
{
    if (preceding != NULL) {
        preceding->optimize();
    }
    if (last_stmt != NULL) {
        last_stmt->optimize();
    }
}


/* Optimize a list of expressions. */
void ast_expr_list::optimize()
{
    /* Your code here */
	if (preceding != NULL) {
		preceding->optimize();
	}
	if (last_expr != NULL) {
		last_expr->optimize();
	}
}


/* Optimize an elsif list. */
void ast_elsif_list::optimize()
{
    /* Your code here */
	if (preceding != NULL) {
		preceding->optimize();
	}
	if (last_elsif != NULL) {
		last_elsif->optimize();
	}
}


/* An identifier's value can change at run-time, so we can't perform
   constant folding optimization on it unless it is a constant.
   Thus we just do nothing here. It can be treated in the fold_constants()
   method, however. */
void ast_id::optimize()
{
}

/*
**	ADDITIONNAL FUNCTIONS
*/

//CHECK IF IS A GIVEN TYPE

int ast_optimizer::is_binary_relation(ast_expression *node)
{
    switch (node->tag)
    {
    case AST_GREATERTHAN:
    case AST_LESSTHAN:
    case AST_EQUAL:
    case AST_NOTEQUAL:
        return true;
    default:
        return false;
    }
}

bool ast_optimizer::is_constant(ast_expression *node)
{
    switch (node->tag)
    {
    case AST_ID:
        return sym_tab->get_symbol_tag(dynamic_cast<ast_id*>(node)->sym_p) == SYM_CONST;
    default:
        return false;
    }
}

bool ast_optimizer::is_value(ast_expression *node)
{
    switch (node->tag)
    {
    case AST_ID:
        return is_constant(node);
    case AST_INTEGER:
    case AST_REAL:
        return true;

    default:
        return false;
    }
}

bool ast_optimizer::is_real(ast_expression *node)
{
   switch (node->tag)
    {
    case AST_ID:
        return sym_tab->get_symbol_type(dynamic_cast<ast_id*>(node)->sym_p) == real_type;
    case AST_REAL:
        return true;
    default:
        return false;
    }
}

//ACCESS (cast) TO VALUE: use of dynamic_cast to access safely to the value and convert with static cast (C++)

float ast_optimizer::get_float(ast_expression *node)
{
    switch (node->tag)
    {
    case AST_ID:
        return static_cast<float>(sym_tab->get_symbol(dynamic_cast<ast_id*>(node)->sym_p)->get_constant_symbol()->const_value.rval);
    case AST_INTEGER:
        return static_cast<float>(dynamic_cast<ast_integer*>(node)->value);
    case AST_REAL:
        return dynamic_cast<ast_real*>(node)->value;
    default:
        fatal("Error in optimization: Not a valid access for float (get_float)");
        return -1;
    }
}

int ast_optimizer::get_integer(ast_expression *node)
{
    switch (node->tag)
    {
    case AST_ID:
        return static_cast<int>(sym_tab->get_symbol(dynamic_cast<ast_id*>(node)->sym_p)->get_constant_symbol()->const_value.ival);
    case AST_INTEGER:
        return dynamic_cast<ast_integer*>(node)->value;
    case AST_REAL:
        return static_cast<int>(dynamic_cast<ast_real*>(node)->value);
    default:
        fatal("Error in optimization: Not a valid access for integer (get_integer)");
        return -1;
    }
}

//CALCULATION OF VALUE: provide functions to do the calculation to optimize the tree

float ast_optimizer::calculate_float(ast_binaryoperation *node)
{
    float left, right;
    left = get_float(node->left);
    right = get_float(node->right);
    switch (node->tag)
    {
    case AST_ADD:
        return left + right;
    case AST_SUB:
        return left - right;
    case AST_MULT:
        return left * right;
    case AST_DIVIDE:
        return left / right;
    default:
        fatal("Error in optimization: Not a valid operation for real (float)");
        return -1;
    }
}

int ast_optimizer::calculate_integer(ast_binaryoperation *node)
{
    int left, right;
    left = get_integer(node->left);
    right = get_integer(node->right);
    switch (node->tag)
    {
    case AST_ADD:
        return left + right;
    case AST_SUB:
        return left - right;
    case AST_OR:
        return left || right;
    case AST_AND:
        return left && right;
    case AST_MULT:
        return left * right;
    case AST_IDIV:
        return left / right;
    case AST_MOD:
        return left % right;
    default:
        fatal("Error in optimization: Not a valid operation for int (integer)");
        return -1;
    }
}

int ast_optimizer::calculate_relation(ast_binaryrelation *node)
{
    int left, right;
    left = get_integer(node->left);
    right = get_integer(node->right);
    switch (node->tag)
    {
    case AST_GREATERTHAN:
        return left > right;
    case AST_LESSTHAN:
        return left < right;
    case AST_EQUAL:
        return left == right;
    case AST_NOTEQUAL:
        return left != right;
    default:
        fatal("Error in optimization: Not a valid operation for relation (relation)");
        return -1;
    }
}

/*
**	END ADDITIONNAL FUNCTIONS
*/

void ast_indexed::optimize()
{
    /* Your code here */
    index->optimize();
    index = optimizer->fold_constants(index);
}

/* This convenience method is used to apply constant folding to all
   binary operations. It returns either the resulting optimized node or the
   original node if no optimization could be performed. */
ast_expression *ast_optimizer::fold_constants(ast_expression *node)
{
    /* Your code here. */
	//result ill be the node that will replace the actual node in case of possible optimization
    ast_expression *result = node;
    if (is_binop(node))
    {
		//Simplify binary operation as possible (binary operation to optimize)
        ast_binaryoperation *binop_toop = dynamic_cast<ast_binaryoperation*>(node);
        if (is_value(binop_toop->left) && is_value(binop_toop->right))
        {
			//If both are values, calculate the operation with left and right value
			//If one value is float at least, the new replacing node is float (or int if not)
            if (is_real(binop_toop->left) || is_real(binop_toop->right))
            {
                float value = calculate_float(binop_toop);
                result = new ast_real(binop_toop->left->pos, value);
            }
            else {
                int value = calculate_integer(binop_toop);
                result = new ast_integer(binop_toop->left->pos, value);
            }
        }
    }
    else if(is_binary_relation(node)) {
		//Simplify binary relation as possible (binary relation to optimize)
        ast_binaryrelation *binrel_toop = dynamic_cast<ast_binaryrelation*>(node);
        if (is_value(binrel_toop->left) && is_value(binrel_toop->right))
        {
            int value = calculate_relation(binrel_toop);
            result = new ast_integer(binrel_toop->left->pos, value);
        }
    }
  	else if (node->tag == AST_CAST) {
		//Simplify the cast operation as possible (cast operation to optimize)
		//Get the type of the element to cast (integer only to cast to real)
    	ast_expression *expr_tocast = node->get_ast_cast()->expr;
		if (expr_tocast->type != void_type){
			result = new ast_real(node->pos, get_float(node->get_ast_cast()->expr));
		}
    }
    return result;
}

/* All the binary operations should already have been detected in their parent
   nodes, so we don't need to do anything at all here. */
void ast_add::optimize()
{
    /* Your code here */
    left->optimize();
    right->optimize();
    left = optimizer->fold_constants(left);
    right = optimizer->fold_constants(right);
}

void ast_sub::optimize()
{
    /* Your code here */
    left->optimize();
    right->optimize();
    left = optimizer->fold_constants(left);
    right = optimizer->fold_constants(right);
}

void ast_mult::optimize()
{
    /* Your code here */
    left->optimize();
    right->optimize();
    left = optimizer->fold_constants(left);
    right = optimizer->fold_constants(right);
}

void ast_divide::optimize()
{
    /* Your code here */
    left->optimize();
    right->optimize();
    left = optimizer->fold_constants(left);
    right = optimizer->fold_constants(right);
}

void ast_or::optimize()
{
    /* Your code here */
    left->optimize();
    right->optimize();
    left = optimizer->fold_constants(left);
    right = optimizer->fold_constants(right);
}

void ast_and::optimize()
{
    /* Your code here */
    left->optimize();
    right->optimize();
    left = optimizer->fold_constants(left);
    right = optimizer->fold_constants(right);
}

void ast_idiv::optimize()
{
    /* Your code here */
    left->optimize();
    right->optimize();
    left = optimizer->fold_constants(left);
    right = optimizer->fold_constants(right);
}

void ast_mod::optimize()
{
    /* Your code here */
    left->optimize();
    right->optimize();
    left = optimizer->fold_constants(left);
    right = optimizer->fold_constants(right);
}



/* We can apply constant folding to binary relations as well. */
void ast_equal::optimize()
{
    /* Your code here */
    left->optimize();
    right->optimize();
    left = optimizer->fold_constants(left);
    right = optimizer->fold_constants(right);
}

void ast_notequal::optimize()
{
    /* Your code here */
    left->optimize();
    right->optimize();
    left = optimizer->fold_constants(left);
    right = optimizer->fold_constants(right);
}

void ast_lessthan::optimize()
{
    /* Your code here */
    left->optimize();
    right->optimize();
    left = optimizer->fold_constants(left);
    right = optimizer->fold_constants(right);
}

void ast_greaterthan::optimize()
{
    /* Your code here */
    left->optimize();
    right->optimize();
    left = optimizer->fold_constants(left);
    right = optimizer->fold_constants(right);
}



/*** The various classes derived from ast_statement. ***/

void ast_procedurecall::optimize()
{
    /* Your code here */
    parameter_list->optimize();
}


void ast_assign::optimize()
{
    /* Your code here */
    rhs->optimize();
    rhs = optimizer->fold_constants(rhs);
}


void ast_while::optimize()
{
    /* Your code here */
    condition->optimize();
    condition = optimizer->fold_constants(condition);

    body->optimize();
}


void ast_if::optimize()
{
    /* Your code here */
    condition->optimize();
    condition = optimizer->fold_constants(condition);

    if(body != NULL)
        body->optimize();

    if(elsif_list != NULL)
    {
      elsif_list->optimize();
    }

    if(else_body != NULL)
    {
      else_body->optimize();
    }
}


void ast_return::optimize()
{
    /* Your code here */
    value->optimize();
    value = optimizer->fold_constants(value);
}


void ast_functioncall::optimize()
{
    /* Your code here */
    parameter_list->optimize();
}

void ast_uminus::optimize()
{
    /* Your code here */
    expr->optimize();
    expr = optimizer->fold_constants(expr);
}

void ast_not::optimize()
{
    /* Your code here */
    expr->optimize();
    expr = optimizer->fold_constants(expr);
}


void ast_elsif::optimize()
{
    /* Your code here */
    condition->optimize();
    condition = optimizer->fold_constants(condition);

    if (body != NULL)
        body->optimize();
}



void ast_integer::optimize()
{
    /* Your code here */
	//Nothing to do (state we want to have)
}

void ast_real::optimize()
{
    /* Your code here */
	//Nothing to do (state we want to have)
}

/* Note: See the comment in fold_constants() about casts and folding. */
void ast_cast::optimize()
{
    /* Your code here */
	//HINT: See in the fold_constants() for AST_CAST
}



void ast_procedurehead::optimize()
{
    fatal("Trying to call ast_procedurehead::optimize()");
}


void ast_functionhead::optimize()
{
    fatal("Trying to call ast_functionhead::optimize()");
}
