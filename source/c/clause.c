#include "clause.h"

uint8_t clause_evaluate(const ClauseCompressed* clause, uint8_t* input, uint32_t n_state, uint32_t n_feature, ModelType type) {
    uint16_t n_pos_literal = clause->n_pos_literal;
    uint16_t n_neg_literal = clause->n_neg_literal;

    char k;
    for (k = 0; k < n_pos_literal; k++)
    {
        uint16_t idx_literal = clause->position[k];
		// Inference model: only included literals are stored
        if (type == MODEL_TYPE__INFERENCE)
        {
            // positive literal is included
            if (input[idx_literal] == 0)
            {
                return 0; // Clause evaluates to false
            }
        }
        
		// Training and compressed models: state > n_state / 2 means the literal is included
        else if (clause->data[k] > n_state / 2)
        {
            // positive literal is included
            if (input[idx_literal] == 0)
            {
                return 0; // Clause evaluates to false
            }
        }
    }

    for (k = 0; k < n_neg_literal; k++)
    {
        uint16_t idx_literal = clause->position[n_pos_literal + k];
		// Inference model: only included literals are stored 
        if (type == MODEL_TYPE__INFERENCE) 
        { 
            // negative literal is included 
            if (input[idx_literal] == 1) { 
                return 0; // Clause evaluates to false 
            } 
        } 
 
        // Training and compressed models: state > n_state / 2 means the literal is included
        else if (clause->data[clause->n_pos_literal + k] > n_state / 2)
        {
            // negative literal is included
            if (input[idx_literal] == 1)
            {
                return 0; // Clause evaluates to false
            }
        }
    }

    return 1; // Clause evaluates to true
}
