#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#include<readline/readline.h>

#include "command.h"
#include "matrix.h"

void run_commands (Commands_t* cmd, Matrix_t** mats, unsigned int num_mats);
int find_matrix_given_name (Matrix_t** mats, unsigned int num_mats,
			const char* target);

// FINISHTODO complete the defintion of this function.

void destroy_remaining_heap_allocations(Matrix_t **mats, unsigned int num_mats);

//FINISHTODO FUNCTION COMMENT5
/*
 * PURPOSE: acts as the control function to get user input and call nessesary functions
 * INPUTS:
 *	argc: the number of arguments passed into the program
 *	argv: array of arguments passed into the program
 * RETURN:
 *  number with the status of the program: 1 for "alls good in here man"
 *										  -1 for "damn, aint going so good"
 *										 -42 for "watching tv and accidently put a dump return value"
 *                                      -757 for "happy hour isn't that far away and sometimes my hand slips"
 **/
int main (int argc, char **argv) {
	srand(time(NULL));
	char *line = NULL;
	Commands_t* cmd;

	Matrix_t *mats[10];
	memset(&mats,0, sizeof(Matrix_t*) * 10); // IMPORTANT C FUNCTION TO LEARN

	Matrix_t *temp = NULL;

	if(!create_matrix (&temp,"temp_mat", 5, 5)){
		perror("PROGRAM FAILED TO CREATE MATRIX");
		return -1;
	} // FINISHTODO ERROR CHECK

	if(add_matrix_to_array(mats,temp, 10) < 0){
		perror("PROGRAM FAILD TO ADD MATRIX TO ARRAY");
		return -1;
	} //FINISHTODO ERROR CHECK NEEDED

	int mat_idx = find_matrix_given_name(mats,10,"temp_mat");

	if (mat_idx < 0) {
		perror("PROGRAM FAILED TO INIT\n");
		return -1;
	}
	random_matrix(mats[mat_idx], 10, 15);

	if(!write_matrix("temp_mat", mats[mat_idx])){
		perror("PROGRAM FAILED TO WRITE TO FILE");
		return -1;
	} // FINISHTODO ERROR CHECK

	line = readline("> ");
	while (strncmp(line,"exit", strlen("exit")  + 1) != 0) {

		if (!parse_user_input(line,&cmd)) {
			printf("Failed at parsing command\n\n");
		}

		if (cmd->num_cmds > 1) {
			run_commands(cmd,mats,10);
		}
		if (line) {
			free(line);
		}
		destroy_commands(&cmd);
		line = readline("> ");
	}
	free(line);
	destroy_remaining_heap_allocations(mats,10);
	return 0;
}

//FINISHTODO FUNCTION COMMENT
/*
 * PURPOSE: run the commands passed in by the user
 * INPUTS:
 *	cmd: pointer to the command structs
 *  mats: pointer to the array that stores the current matrices
 *  num_mats: max number of matrices that the mats array can hold
 * RETURN:
 *  void
 *
 **/
void run_commands (Commands_t* cmd, Matrix_t** mats, unsigned int num_mats) {
	//FINISHTODO ERROR CHECK INCOMING PARAMETERS
	if(!cmd || !mats){
		printf("There was an error with the inputs :)\n");
		return;
	}

	/*Parsing and calling of commands*/
	if (strncmp(cmd->cmds[0],"display",strlen("display") + 1) == 0
		&& cmd->num_cmds == 2) {
			/*find the requested matrix*/
			int idx = find_matrix_given_name(mats,num_mats,cmd->cmds[1]);
			if (idx >= 0) {
				display_matrix (mats[idx]);
			}
			else {
				printf("Matrix (%s) doesn't exist\n", cmd->cmds[1]);
				return;
			}
	}
	else if (strncmp(cmd->cmds[0],"add",strlen("add") + 1) == 0
		&& cmd->num_cmds == 4) {
			int mat1_idx = find_matrix_given_name(mats,num_mats,cmd->cmds[1]);
			int mat2_idx = find_matrix_given_name(mats,num_mats,cmd->cmds[2]);
			if (mat1_idx >= 0 && mat2_idx >= 0) {
				Matrix_t* c = NULL;
				if( !create_matrix (&c,cmd->cmds[3], mats[mat1_idx]->rows,
						mats[mat1_idx]->cols)) {
					printf("Failure to create the result Matrix (%s)\n", cmd->cmds[3]);
					return;
				}

				if(add_matrix_to_array(mats,c, num_mats) < 0){
					printf("Failure to add the new matrix to the array");
					return;
				} //FINISHTODO ERROR CHECK NEEDED


				if (! add_matrices(mats[mat1_idx], mats[mat2_idx],c) ) {
					printf("Failure to add %s with %s into %s\n", mats[mat1_idx]->name, mats[mat2_idx]->name,c->name);
					return;
				}
				printf ("Addition of %s into %s finished\n", mats[mat1_idx]->name, mats[mat2_idx]->name);
			}
	}
	else if (strncmp(cmd->cmds[0],"duplicate",strlen("duplicate") + 1) == 0
		&& cmd->num_cmds == 3 && strlen(cmd->cmds[1]) + 1 <= MATRIX_NAME_LEN) {
		int mat1_idx = find_matrix_given_name(mats,num_mats,cmd->cmds[1]);
		if (mat1_idx >= 0 ) {
				Matrix_t* dup_mat = NULL;
				if( !create_matrix (&dup_mat,cmd->cmds[2], mats[mat1_idx]->rows,
						mats[mat1_idx]->cols)) {
					return;
				}
				if(!duplicate_matrix (NULL, dup_mat)){ //mats[mat1_idx]
					printf("Failure to duplicate the matrix");
					return;
				} //FINISHTODO ERROR CHECK NEEDED

				if(add_matrix_to_array(mats,dup_mat,num_mats) < 0){
					printf("Failure to add the new matrix to the array");
					return;
				} //FINISHTODO ERROR CHECK NEEDED

				printf ("Duplication of %s into %s finished\n", mats[mat1_idx]->name, cmd->cmds[2]);
		}
		else {
			printf("Duplication Failed\n");
			return;
		}
	}
	else if (strncmp(cmd->cmds[0],"equal",strlen("equal") + 1) == 0
		&& cmd->num_cmds == 3) {
			int mat1_idx = find_matrix_given_name(mats,num_mats,cmd->cmds[1]);
			int mat2_idx = find_matrix_given_name(mats,num_mats,cmd->cmds[2]);
			if (mat1_idx >= 0 && mat2_idx >= 0) {
				if ( equal_matrices(mats[mat1_idx],mats[mat2_idx]) ) {
					printf("SAME DATA IN BOTH\n");
				}
				else {
					printf("DIFFERENT DATA IN BOTH\n");
				}
			}
			else {
				printf("Equal Failed\n");
				return;
			}
	}
	else if (strncmp(cmd->cmds[0],"shift",strlen("shift") + 1) == 0
		&& cmd->num_cmds == 4) {
		int mat1_idx = find_matrix_given_name(mats,num_mats,cmd->cmds[1]);
		const int shift_value = atoi(cmd->cmds[3]);
		if (mat1_idx >= 0 ) {
			if(!bitwise_shift_matrix(mats[mat1_idx],cmd->cmds[2][0], shift_value)){
				printf("Failure to shift the matrix");
				return;
			} //FINISHTODO ERROR CHECK NEEDED

			printf("Matrix (%s) has been shifted by %d\n", mats[mat1_idx]->name, shift_value);

		}
		else {
			printf("Matrix shift failed\n");
			return;
		}

	}
	else if (strncmp(cmd->cmds[0],"read",strlen("read") + 1) == 0
		&& cmd->num_cmds == 2) {
		Matrix_t* new_matrix = NULL;
		if(! read_matrix(cmd->cmds[1],&new_matrix)) {
			printf("Read Failed\n");
			return;
		}

		if(add_matrix_to_array(mats,new_matrix, num_mats) < 0){
			printf("Failure to add the new matrix to the array");
			return;
		} //FINISHTODO ERROR CHECK NEEDED
		printf("Matrix (%s) is read from the filesystem\n", cmd->cmds[1]);
	}
	else if (strncmp(cmd->cmds[0],"write",strlen("write") + 1) == 0
		&& cmd->num_cmds == 2) {
		int mat1_idx = find_matrix_given_name(mats,num_mats,cmd->cmds[1]);
		if(mat1_idx < 0 || !write_matrix(mats[mat1_idx]->name,mats[mat1_idx])) {
			printf("Write Failed\n");
			return;
		}
		else {
			printf("Matrix (%s) is wrote out to the filesystem\n", mats[mat1_idx]->name);
		}
	}
	else if (strncmp(cmd->cmds[0], "create", strlen("create") + 1) == 0
		&& strlen(cmd->cmds[1]) + 1 <= MATRIX_NAME_LEN && cmd->num_cmds == 4) {
		Matrix_t* new_mat = NULL;
		const unsigned int rows = atoi(cmd->cmds[2]);
		const unsigned int cols = atoi(cmd->cmds[3]);

		if(!create_matrix(&new_mat,cmd->cmds[1],rows, cols)){
			printf("Failure to create matrix");
			return;
		} //FINISHTODO ERROR CHECK NEEDED

		if(add_matrix_to_array(mats,new_mat,num_mats) < 0){
			printf("Failure to add the new matrix to the array");
			return;
		} // FINISHTODO ERROR CHECK NEEDED

		printf("Created Matrix (%s,%u,%u)\n", new_mat->name, new_mat->rows, new_mat->cols);
	}
	else if (strncmp(cmd->cmds[0], "random", strlen("random") + 1) == 0
		&& cmd->num_cmds == 4) {
		int mat1_idx = find_matrix_given_name(mats,num_mats,cmd->cmds[1]);
		const unsigned int start_range = atoi(cmd->cmds[2]);
		const unsigned int end_range = atoi(cmd->cmds[3]);

		if(mat1_idx < 0 || !random_matrix(mats[mat1_idx],start_range, end_range)){
			printf("Failure in creating random numbers for the matrix");
			return;
		} //FINISHTODO ERROR CHECK NEEDED

		printf("Matrix (%s) is randomized between %u %u\n", mats[mat1_idx]->name, start_range, end_range);
	}
	else {
		printf("Not a command in this application\n");
	}

}

//FINIISHTODO FUNCTION COMMENT
/*
 * PURPOSE: checks if the passed in name is in the array of stored matrices
 * INPUTS:
 *	mats: double pointer to the struct witht he array of matrices
 * RETURN:
 *  void
 *
 * NOTE:: had to add check in for loop to stop seg fault if matrix not found
 **/
int find_matrix_given_name (Matrix_t** mats, unsigned int num_mats, const char* target) {
	//TODO ERROR CHECK INCOMING PARAMETERS

	//NOTE: Tried to check

	if(!mats || !target){
		return -1;
	}

	for (int i = 0; i < num_mats; ++i) {
		if (mats[i] && strncmp(mats[i]->name,target,strlen(mats[i]->name)) == 0) {
			return i;
		}
	}
	return -1;
}

//FINISHTODO FUNCTION COMMENT
/*
 * PURPOSE: free the memory that holds all the commands
 * INPUTS:
 *	mats: pointer to the array that holds the pointers
 *	num_mats: the max number of matrices in the array
 * RETURN:
 *  void
 *
 **/
void destroy_remaining_heap_allocations(Matrix_t **mats, unsigned int num_mats) {

	if(!mats){
		return;
	}

	for(int i = 0; i < num_mats; i++){
		if(mats[i]){
			free(mats[i]->data);
			free(mats[i]);
		}
	}
}
