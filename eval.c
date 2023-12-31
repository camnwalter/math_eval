#include "eval.h"
#include "lexer.h"
#include "ast.h"
#include <math.h>
#include <stdio.h>

static bool eval_node(struct node *node) {
    if (node == NULL) {
        return true;
    }

    switch (node->node_type) {
        case INTERNAL:
            eval_node(node->left);
            if (is_unary_op(node->token)) {
                switch (node->token) {
                    case U_MINUS:
                        node->val = -node->left->val;
                        break;
                    default:
                        break;
                }
            }
            if (is_binary_op(node->token)) {
                eval_node(node->right);

                switch (node->token) {
                    case PLUS:
                        node->val = node->left->val + node->right->val;
                        break;
                    case MINUS:
                        node->val = node->left->val - node->right->val;
                        break;
                    case MULTIPLY:
                        node->val = node->left->val * node->right->val;
                        break;
                    case DIVIDE:
                        if (node->right->val != 0) {
                            node->val = node->left->val / node->right->val;
                        } else {
                            puts("Error: Can't divide by 0");
                            return false;
                        }
                        break;
                    case POWER:
                        node->val = pow(node->left->val, node->right->val);
                        break;
                    default:
                        break;
                }
            }
            break;
        case LEAF: // value is already there, so do nothing
        default:
            break;
    }
    return true;
}

static void eval_root(struct node *root) {
    if (root == NULL) {
        return;
    }

    if (eval_node(root)) {
        printf("%.12g\n", root->val);
    }
}

void eval(char *raw_input) {
    struct list *tokens = list_init(0);
    if (convert_to_tokens(tokens, raw_input)) { // lexer
        struct node *root = NULL;
        if (parse_root(tokens, &root)) { // parser
            eval_root(root);
        }

        cleanup(root);
    }

    list_free(tokens);
}
