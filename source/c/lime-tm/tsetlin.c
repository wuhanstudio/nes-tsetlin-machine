#include "tsetlin.h"

#if defined(__ZEPHYR__)
    /* Zephyr RTOS */
    #include <zephyr/fs/fs.h>
    LOG_MODULE_REGISTER(tsetlin);
#endif

int tsetlin_evaluate(Tsetlin* model, char* input, int *out_votes, char* out_class) {
    char c;
    // Find class with maximum votes
    char max_class = 0;
    int max_votes = out_votes[0];

    // memset(out_votes, 0, model->n_class * sizeof(int));
    for (c = 0; c < model->n_class; c++)
    {
        out_votes[c] = 0;
    }

    for (c = 0; c < model->n_class; c++)
    {
        char j;
        for (j = 0; j <(char) model->n_clause / 2; j++)
        {
            const ClauseCompressed* p_clause = &model->clauses_compressed[c * model->n_clause + j * 2];
            const ClauseCompressed* n_clause = &model->clauses_compressed[c * model->n_clause + j * 2 + 1];

            out_votes[c] += clause_evaluate(p_clause, input, model->n_state, model->n_feature, model->model_type);
            out_votes[c] -= clause_evaluate(n_clause, input, model->n_state, model->n_feature, model->model_type);
        }
    }

    for (c = 1; c < model->n_class; c++)
    {
        if (out_votes[c] > max_votes)
        {
            max_votes = out_votes[c];
            max_class = c;
        }
    }

    *out_class = max_class;

    return 0;
}
