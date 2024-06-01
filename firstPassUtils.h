void tokenizeLine(char *line, char **tokens, int *num_tokens);

void tag2Table(char** Iarr, char** Darr, char *Ltype, char* line, int *word_num, HashTable *Atable, HashTable *Symbol_Table, int *DC, int *IC, char *token, char **tokens, int *num_tokens);

int operand_num(char **tokens, int *num_tokens, HashTable *Atable);

bool isCommentOrSpace (char *str);

bool isTag (char *str);

char *lineType (char *word, HashTable *Atable);

int count_lines(FILE *file);

void actionTable (HashTable *Atable);

bool isRegister (char *str);

int num_of_words(char *token, HashTable *Atable, char *str, char *Ltype, int *DC);

int type2num (char *Ltype);

char *opMethod (char *token);

bool isBranch (char *opcode);

void processString(char *str_arg, char **arr, int *Counter);

void Asm2Machine(char **arr, char **tokens, int num_tokens, int *Counter, HashTable *Atable, HashTable *Symbol_Table,  char *Ltype);