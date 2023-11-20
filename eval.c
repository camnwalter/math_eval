#include "eval.h"
#include "lexer.h"
#include "ast.h"
#include <math.h>
#include <stdio.h>

static void eval_node(struct node *node) {
    if (node == NULL) {
        return;
    }

    switch (node->node_type) {
        case INTERNAL:
            eval_node(node->children[0]);
            if (is_unary_op(node->token)) {
                switch (node->token) {
                    case U_MINUS:
                        node->val = -node->children[0]->val;
                        break;
                    default:
                        break;
                }
            }
            if (is_binary_op(node->token)) {
                eval_node(node->children[1]);

                switch (node->token) {
                    case PLUS:
                        node->val = node->children[0]->val + node->children[1]->val;
                        break;
                    case MINUS:
                        node->val = node->children[0]->val - node->children[1]->val;
                        break;
                    case MULTIPLY:
                        node->val = node->children[0]->val * node->children[1]->val;
                        break;
                    case DIVIDE:
                        if (node->children[1]->val != 0) {
                            node->val = node->children[0]->val / node->children[1]->val;
                        } else {
                            // TODO: handle errors
                        }
                        break;
                    case POWER:
                        node->val = pow(node->children[0]->val, node->children[1]->val);
                        break;
                    default:
                        break;
                }
            }
            if (node->token == IDENTITY) {
                node->val = node->children[0]->val;
            }
            break;
        case LEAF:
        default:
            break;
    }
}

static void eval_root(struct node *root) {
    if (root == NULL) {
        return;
    }

    eval_node(root);
    printf("%.2f\n", root->val);
}

void eval(char *raw_input) {
    struct list *tokens = convert_to_tokens(raw_input); // lexer
    struct node *root = parse_root(tokens); // parser
    eval_root(root);

    cleanup(root);
    list_free(tokens);
}
