#include "../HEADERS/Tree_Print_Ops.h"

static int counter_dumps = 0;

tree_error_t print_tree(FILE* file, tree_t* tree, tree_print_node_value printer)
{
    #ifdef DEBUG
    if (file == NULL)
    {
        tree->error |= TREE_FILE_NOT_OPEN;
        printf("File don`t open\n");
        return TREE_ERROR;
    }
    #endif 

    size_t deep = 0;
    tree_error_t res_print_nodes = print_node(file, tree, tree->root, deep, printer);

    #ifdef DEBUG
    if (res_print_nodes == TREE_ERROR)
    {
        return TREE_ERROR;
    }
    #endif

    return TREE_SUCCESS;
}

tree_error_t print_node(FILE* file, tree_t* tree, const node_t* node, size_t deep, tree_print_node_value printer)
{
    #ifdef DEBUG
    if (file == NULL)
    {
        tree->error |= TREE_FILE_NOT_OPEN;
        printf("File don`t open\n");
        return TREE_ERROR;
    }

    if ((node->left == NULL && node->right != NULL) || (node->right == NULL && node->left != NULL))
    {
        tree->error |= TREE_INCORRECT_NODE;
        return TREE_ERROR;
    }
    #endif 

    fprintf(file, "%*s(", (int) deep, " ");
    tree_error_t res_printer = printer(file, node->value);
    fprintf(file, "\n");

    #ifdef DEBUG
    if (res_printer == TREE_ERROR)
    {
        return TREE_ERROR;
    }
    #endif

    if (node->left == NULL && node->right == NULL)
    {
        fprintf(file, "%*s)\n", (int) deep, " ");
        return TREE_SUCCESS;
    }
 
    tree_error_t res_print_left = print_node(file, tree, node->left, deep + 4, printer);
    tree_error_t res_print_right = print_node(file, tree, node->right, deep + 4, printer);

    #ifdef DEBUG
    if (res_print_left == TREE_ERROR || res_print_right == TREE_ERROR)
    {
        return TREE_ERROR;
    }
    #endif 

    fprintf(file, "%*s)\n", (int) deep, " ");

    return TREE_SUCCESS;
}

tree_error_t print_str(FILE* file, const void* value)
{
    #ifdef DEBUG
    if (value == NULL || file == NULL) 
    {
        return TREE_ERROR;
    }
    #endif

    const char* str = (const char*) value;
    fprintf(file, "\"%s\"", str);

    return TREE_SUCCESS;
}

#ifdef DEBUG
void create_nodes_dot_file(FILE* dot, const node_t* node)
{
    fprintf(dot, "n%p [fillcolor=\"#a094ef\", label=<<table border=\"0\" cellborder=\"0\" cellspacing=\"0\" cellpadding=\"4\">\n"
                 "<tr>"
                 "  <td colspan=\"2\" border=\"1\" sides=\"b\">%p</td>\n"
                 "</tr>\n"
                 "<tr>"
                 "  <td colspan=\"2\" border=\"1\" sides=\"b\"><b>%s</b></td>\n"
                 "</tr>\n"
                 "<tr>"
                 "  <td border=\"1\" sides=\"r\"><b>LEFT:</b> %p</td>\n"
                 "  <td><b>RIGHT:</b> %p</td>\n"
                 "</tr>\n"
                 "</table>>];\n", 
                 node, node, (char*) node->value, node->left, node->right);

    if (node->left == NULL && node->right == NULL)
    {
        return;
    }

    create_nodes_dot_file(dot, node->left);
    create_nodes_dot_file(dot, node->right);

    fprintf(dot, "n%p -> n%p [label = \"yes\"];\n", node, node->left);
    fprintf(dot, "n%p -> n%p [label = \"no\"];\n", node, node->right);
}

void create_tree_dot_file(FILE* dot, const tree_t* tree)
{
    fprintf(dot, "/* Auto generate code */\n\n");
    fprintf(dot, "digraph G {\n");
    fprintf(dot, "bgcolor=\"#fbecff\";\n"
                 "forcelabels=true;\n"
                 "ordering=out;\n"
                 "nodesep=1.;\n"
                 "ranksep=1.;\n");
    fprintf(dot, "node [fontsize=12, shape=box, style=\"filled, rounded\", color=\"#4a3056\", fontname=\"DejaVu Sans\"];\n");
    fprintf(dot, "edge [labelfloat=false];\n\n");

    create_nodes_dot_file(dot, tree->root);

    fprintf(dot, "}\n");
}

void tree_dump(FILE* html, const tree_t* tree, const char* file_name, const char* func_name, int line)
{
    FILE* log = fopen(LOG_FILE, "w");
    // PRINT_LIST_ERROR((int) list->error);
    // list_logger(log, list->error);

    fprintf(log,"Dump was summoned in\n"
                "      file: == %s ==\n"       
                "      function: == %s ==\n"      
                "      line: == %d ==\n", 
            file_name, func_name, line);
    fflush(log);
    fprintf(log,"\nCurrent parametres:\n");

    fprintf(log,"====== struct TREE_T ======\n"
                "       list: [%p]    \n"
                "       root: [%p]    \n\n\n",
                tree, tree->root);
    fprintf(log, "===== DATA =====\n");

// PRINT DATA -----------------------------------------------------------------------

    print_nodes_into_logs(log, tree->root);

// GRAPHIC DUMP ---------------------------------------------------------------------

    FILE* dot = fopen(DOT_FILE, "w");
    create_tree_dot_file(dot, tree);
    fclose(dot);

    char file_name_listing[1000] = {};
    char script[1030] = {};
    
    snprintf(file_name_listing, 1000, "report/tree%d.png",  counter_dumps);
    snprintf(script, 1030, "dot -Tpng " DOT_FILE " -o BIN_TREE/%s", file_name_listing);

    system(script);

// HTML --------------------------------------------------------------------------------------

    fflush(log);

    FILE* text_file = fopen(LOG_FILE, "r");
    fprintf(html, "<img src=\"%s\" width=\"50%%\" alt=\"Dump %d\">\n", file_name_listing, counter_dumps);
    fprintf(html, "<pre>\n");

    int c = 0;

    while ((c = fgetc(text_file)) != EOF)
    {
        fprintf(html, "%c", c);
    }

    fprintf(html, "</pre>\n");

    fclose(text_file);
    fclose(log);

    counter_dumps++;
}

void print_nodes_into_logs(FILE* log, node_t* node)
{
    fprintf(log, "node [ %p ]\n"
                 "value: %s\n"
                 "left node [ %p ]\n"
                 "right node [ %p ]\n\n\n",
                node, (char*) node->value, node->left, node->right);

    if (node->left != NULL && node->right != NULL)
    {
        print_nodes_into_logs(log, node->left);
        print_nodes_into_logs(log, node->right);
    }

    return;
}

#endif

