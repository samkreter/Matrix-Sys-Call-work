#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>


#include "matrix.h"


#define MAX_CMD_COUNT 50

/*protected functions*/
void load_matrix (Matrix_t* m, unsigned int* data);

/*
 * PURPOSE: instantiates a new matrix with the passed name, rows, cols
 * INPUTS:
 *	name the name of the matrix limited to 50 characters
 *  rows the number of rows the matrix
 *  cols the number of cols the matrix
 * RETURN:
 *  If no errors occurred during instantiation then true
 *  else false for an error in the process.
 *
 **/

bool create_matrix (Matrix_t** new_matrix, const char* name, const unsigned int rows,
						const unsigned int cols) {

	//FINISHTODO ERROR CHECK INCOMING PARAMETERS
	//check for null values
	//Since rows and cols are unsigned no need to check if they're negitive
	if(!new_matrix || !name){
		return false;
	}

	//####################################

	*new_matrix = calloc(1,sizeof(Matrix_t));
	if (!(*new_matrix)) {
		return false;
	}
	(*new_matrix)->data = calloc(rows * cols,sizeof(unsigned int));
	if (!(*new_matrix)->data) {
		return false;
	}
	(*new_matrix)->rows = rows;
	(*new_matrix)->cols = cols;
	unsigned int len = strlen(name) + 1;
	if (len > MATRIX_NAME_LEN) {
		return false;
	}
	strncpy((*new_matrix)->name,name,len);
	return true;

}

	//FINISHTODO FUNCTION COMMENT
 /*
 * PURPOSE: free the memory for the matrix array
 * INPUTS:
 *	m: pointer to the matrix array to be freed
 * RETURN:
 *  void
 *
 **/
void destroy_matrix (Matrix_t** m) {

	//FINISHTODO ERROR CHECK INCOMING PARAMETERS
	if(!m){
		return;
	}
	//####################################

	free((*m)->data);
	free(*m);
	*m = NULL;
}



	//FINISHTODO FUNCTION COMMENT
 /*
 * PURPOSE: check if the data in the two matrices passed in are equal
 * INPUTS:
 *	a: pointer to  a matrix to be compared to
 *  b: pointer to second matrix to be compared to
 * RETURN:
 *  If no errors and the data of the two matrices are equal then true
 *  else false.
 *
 **/
bool equal_matrices (Matrix_t* a, Matrix_t* b) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if (!a || !b || !a->data || !b->data) {
		return false;
	}
	//#####################################


	int result = memcmp(a->data,b->data, sizeof(unsigned int) * a->rows * a->cols);
	if (result == 0) {
		return true;
	}
	return false;
}

	//TODO FUNCTION COMMENT
 /*
 * PURPOSE: duplicate the data from the src matrix to the dest matrix
 * INPUTS:
 *	src: pointer to the matrix of the origiinal matrix
 *  dest: pointer to the matrix to copy the data form src matrix to
 * RETURN:
 *  If no errors with input and matrix are duplicated and checked then true
 *  else false for the input errors or not equal matrixs after duplication.
 *
 **/
bool duplicate_matrix (Matrix_t* src, Matrix_t* dest) {


	//TODO ERROR CHECK INCOMING PARAMETERS

	if (!src || !dest) {
		return false;
	}
	//####################################

	/*
	 * copy over data
	 */
	unsigned int bytesToCopy = sizeof(unsigned int) * src->rows * src->cols;
	memcpy(dest->data,src->data, bytesToCopy);

	return equal_matrices (src,dest);
}

	//TODO FUNCTION COMMENT
 /*
 * PURPOSE: Performs either a left or right shift on all elements in the passed in matrix
 * INPUTS:
 *	a: pointer to the matrix to be shifted
 *  direction: letter to tell which direction to shift to
 *  shift: the number of bits to be shifted
 * RETURN:
 *  If no errors with input then true
 *  else false for the input errors.
 *
 **/
bool bitwise_shift_matrix (Matrix_t* a, char direction, unsigned int shift) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if (!a || !a->rows || !a->cols) {
		return false;
	}
	//####################################

	if (direction == 'l') {
		unsigned int i = 0;
		for (; i < a->rows; ++i) {
			unsigned int j = 0;
			for (; j < a->cols; ++j) {
				a->data[i * a->cols + j] = a->data[i * a->cols + j] << shift;
			}
		}

	}
	else {
		unsigned int i = 0;
		for (; i < a->rows; ++i) {
			unsigned int j = 0;
			for (; j < a->cols; ++j) {
				a->data[i * a->cols + j] = a->data[i * a->cols + j] >> shift;
			}
		}
	}

	return true;
}

	//TODO FUNCTION COMMENT
 /*
 * PURPOSE: adds the data at the same element of two matrices passed and places
 			in same element of the c matrix
 * INPUTS:
 *	a: pointer to the matrix to be added with second matrix
 *  b: pointer to the matrix to be added to the first matrix
 *  c: pointer to the matrix to store the result of the sum
 * RETURN:
 *  If no errors with input and the row and column sizes are the same then true
 *  else false.
 *
 **/
bool add_matrices (Matrix_t* a, Matrix_t* b, Matrix_t* c) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(a == NULL || b == NULL || c == NULL){
		printf("hey add matrix");
		return false;
	}
	//####################################

	if (a->rows != b->rows && a->cols != b->cols) {
		return false;
	}

	for (int i = 0; i < a->rows; ++i) {
		for (int j = 0; j < b->cols; ++j) {
			c->data[i * a->cols +j] = a->data[i * a->cols + j] + b->data[i * a->cols + j];
		}
	}
	return true;
}

	//TODO FUNCTION COMMENT
 /*
 * PURPOSE: displays the data from the passed in matrix to the console
 * INPUTS:
 *	m: pointer to the matrix to be displayed
 * RETURN:
 *  void
 *
 **/
void display_matrix (Matrix_t* m) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(!m || !m->rows || !m->cols){
		return;
	}
	//###################################

	printf("\nMatrix Contents (%s):\n", m->name);
	printf("DIM = (%u,%u)\n", m->rows, m->cols);
	for (int i = 0; i < m->rows; ++i) {
		for (int j = 0; j < m->cols; ++j) {
			printf("%u ", m->data[i * m->cols + j]);
		}
		printf("\n");
	}
	printf("\n");

}

	//TODO FUNCTION COMMENT
 /*
 * PURPOSE: reads the name, and data from a file then calls load matrix to create the matrix
 * INPUTS:
 *	matrix_input_filename: pointer to the name of the file to open
 *  m: pointer to a pointer to the matrix that the new read in data with be loaded into
 * RETURN:
 *  If no errors with input or reading in the data from the file then true
 *  else false.
 *
 **/
bool read_matrix (const char* matrix_input_filename, Matrix_t** m) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(!matrix_input_filename || !m){
		return false;
	}
	//#####################################


	int fd = open(matrix_input_filename,O_RDONLY);
	if (fd < 0) {
		printf("FAILED TO OPEN FOR READING\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}

	/*read the wrote dimensions and name length*/
	unsigned int name_len = 0;
	unsigned int rows = 0;
	unsigned int cols = 0;

	if (read(fd,&name_len,sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READING FILE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}
	char name_buffer[50];
	if (read (fd,name_buffer,sizeof(char) * name_len) != sizeof(char) * name_len) {
		printf("FAILED TO READ MATRIX NAME\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	if (read (fd,&rows, sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READ MATRIX ROW SIZE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	if (read(fd,&cols,sizeof(unsigned int)) != sizeof(unsigned int)) {
		printf("FAILED TO READ MATRIX COLUMN SIZE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	unsigned int numberOfDataBytes = rows * cols * sizeof(unsigned int);
	unsigned int *data = calloc(rows * cols, sizeof(unsigned int));
	if (read(fd,data,numberOfDataBytes) != numberOfDataBytes) {
		printf("FAILED TO READ MATRIX DATA\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}

		return false;
	}

	if (!create_matrix(m,name_buffer,rows,cols)) {
		return false;
	}

	load_matrix(*m,data);
	free(data);
	if (close(fd)) {
		return false;

	}
	return true;
}

	//TODO FUNCTION COMMENT
 /*
 * PURPOSE: create buffer from the data in the passed in matrix then write to a file
 * INPUTS:
 *	m: pointer to the matrix to write its data to a file
 *  matrix_output_filename: name of the file to write data out to
 * RETURN:
 *  If no errors with opening and writing file then true
 *  else false with errors.
 *
 **/
bool write_matrix (const char* matrix_output_filename, Matrix_t* m) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(!matrix_output_filename || !m){
		return false;
	}
	//####################################

	int fd = open (matrix_output_filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	/* ERROR HANDLING USING errorno*/
	if (fd < 0) {
		printf("FAILED TO CREATE/OPEN FILE FOR WRITING\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXISTS\n");
		}
		return false;
	}
	/* Calculate the needed buffer for our matrix */
	unsigned int name_len = strlen(m->name) + 1;
	unsigned int numberOfBytes = sizeof(unsigned int) + (sizeof(unsigned int)  * 2) + name_len + sizeof(unsigned int) * m->rows * m->cols + 1;
	/* Allocate the output_buffer in bytes
	 * IMPORTANT TO UNDERSTAND THIS WAY OF MOVING MEMORY
	 */
	unsigned char* output_buffer = calloc(numberOfBytes,sizeof(unsigned char));
	unsigned int offset = 0;
	memcpy(&output_buffer[offset], &name_len, sizeof(unsigned int)); // IMPORTANT C FUNCTION TO KNOW
	offset += sizeof(unsigned int);
	memcpy(&output_buffer[offset], m->name,name_len);
	offset += name_len;
	memcpy(&output_buffer[offset],&m->rows,sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy(&output_buffer[offset],&m->cols,sizeof(unsigned int));
	offset += sizeof(unsigned int);
	memcpy (&output_buffer[offset],m->data,m->rows * m->cols * sizeof(unsigned int));
	offset += (m->rows * m->cols * sizeof(unsigned int));
	output_buffer[numberOfBytes - 1] = EOF;

	if (write(fd,output_buffer,numberOfBytes) != numberOfBytes) {
		printf("FAILED TO WRITE MATRIX TO FILE\n");
		if (errno == EACCES ) {
			perror("DO NOT HAVE ACCESS TO FILE\n");
		}
		else if (errno == EADDRINUSE ){
			perror("FILE ALREADY IN USE\n");
		}
		else if (errno == EBADF) {
			perror("BAD FILE DESCRIPTOR\n");
		}
		else if (errno == EEXIST) {
			perror("FILE EXIST\n");
		}
		return false;
	}

	if (close(fd)) {
		return false;
	}
	free(output_buffer);

	return true;
}

	//TODO FUNCTION COMMENT
 /*
 * PURPOSE: add Random unsigned ints to the passed in matrix
 * INPUTS:
 *	m: pointer to the matrix to add the random numbers to
 *  start_range: the start of the range for the random numbers to be in
 *  end_range: the end range that the random number will be in
 * RETURN:
 *  If no errors with input then true
 *  else false for the input errors.
 *
 **/
bool random_matrix(Matrix_t* m, unsigned int start_range, unsigned int end_range) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(!m || end_range < start_range || !m->rows || !m->cols){
		return false;
	}

	for (unsigned int i = 0; i < m->rows; ++i) {
		for (unsigned int j = 0; j < m->cols; ++j) {
			m->data[i * m->cols + j] = rand() % (end_range + 1 - start_range) + start_range;
		}
	}
	return true;
}

/*Protected Functions in C*/

	//TODO FUNCTION COMMENT
 /*
 * PURPOSE: add data to the passed in matrix
 * INPUTS:
 *	m: pointer to the matrix to add the data to
 *  data: data to be added to the matrix
 * RETURN:
 *  void
 *
 **/
void load_matrix (Matrix_t* m, unsigned int* data) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(m == NULL || data == NULL){
		return;
	}
	//####################################

	memcpy(m->data,data,m->rows * m->cols * sizeof(unsigned int));
}

	//TODO FUNCTION COMMENT
 /*
 * PURPOSE: add newley created matrix to the array of matrices
 * INPUTS:
 *	mats: pointer to the array of matrices
 *  new_matrix: newly created matrix to be added to the array
 *  num_mats: number of matrixs already in the array
 * RETURN:
 *  If no errors with input then return position of new matrix
 *  else -1 for input erros
 *
 **/
int add_matrix_to_array (Matrix_t** mats, Matrix_t* new_matrix, unsigned int num_mats) {

	//TODO ERROR CHECK INCOMING PARAMETERS
	if(!mats || !new_matrix){
		return -1;
	}

	static long int current_position = 0;
	const long int pos = current_position % num_mats;
	if ( mats[pos] ) {
		destroy_matrix(&mats[pos]);
	}
	mats[pos] = new_matrix;
	current_position++;
	return pos;
}
