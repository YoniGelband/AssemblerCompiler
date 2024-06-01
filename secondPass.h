struct HashTable;
int secondary_pass (HashTable *Atable, HashTable *Symbol_Table, int num_tokens, char *filename);
int Asm2Machine2(char **tokens, int IC, int num_tokens , HashTable *Symbol_Table, HashTable *Atable);