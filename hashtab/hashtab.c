#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 2 << 5


#define bool unsigned int
#define true 1
#define false 0

unsigned int DJBHash(char * str) {
	unsigned int i;
	unsigned int hash = 5381;
	size_t len = strlen(str);
	for(i = 0; i < len; ++i) {
		hash = ((hash << 5) + hash) + (str[i]);
	}

	return hash;
}


typedef struct hash_item {
	char * key;
	char * value;
	void * next;
} hash_item;


size_t hash_write(hash_item ** hash, size_t hash_size, char * key, char * value) {
	size_t index = DJBHash(key) % hash_size;


	hash_item ** item_pointer = &hash[index];

	while(true) {
		if(*item_pointer == NULL) {
			// create new key
			*item_pointer = malloc(sizeof(hash_item));
			break;
		} else if (strcmp((*item_pointer)->key, key) == 0) {
			// use existing key
			break;
		} else {
			// go to next key in chain
			item_pointer = (hash_item **) &((*item_pointer)->next);
		}
	}

	hash_item * item = *item_pointer;

	item->key = malloc(sizeof(char) * strlen(key));
	strcpy(item->key, key);

	item->value = malloc(sizeof(char) * strlen(value));
	strcpy(item->value, value);
	
	item->next = NULL;

	return index;
}

hash_item * hash_read(hash_item ** hash, size_t hash_size, char * key) {
	size_t index = DJBHash(key) % hash_size;

	hash_item * item = hash[index];

	while (item != NULL && strcmp(item->key, key) != 0) {
		item = item->next;
	}

	return item;
}

bool hash_delete(hash_item ** hash, size_t hash_size, char *key) {
	size_t index = DJBHash(key) % hash_size;


	hash_item ** prev = &hash[index];
	hash_item * item = hash[index];

	while(item != NULL) {
		if(strcmp(item->key, key) == 0) {
			*prev = item->next;
			free(item);
			return true;
		}

		prev = (hash_item **) &item->next;
		item = item->next;
	}


	return false;
}


void hash_dump(hash_item ** hash, size_t hash_size) {

	hash_item * chain;
	size_t i;
	for(i = 0; i < hash_size; ++i) {
		chain = hash[i];

		printf("[%lu]", i);

		while(true) {
			if(chain == NULL) {
				printf(" -> null");
				break;
			} else {
				printf(" -> %s=%s", chain->key, chain->value);
				chain = chain->next;
			}
		}

		printf("\n");
	}
}


int main(int argc, char ** argv) {
	int i;

	// init hash table
	size_t hash_size = HASH_SIZE;
	hash_item ** hash_tab = malloc(sizeof(hash_item *) * hash_size);

	// remove garbage from hash_tab
	for(i = 0; i < hash_size; ++i) {
		hash_tab[i] = NULL;
	}

	// read command
	char * command = NULL;
	size_t len;
	char * DELIM = " \t";
	char * arg;
	char * key;
	char * value;


	while(true) {
		getline(&command, &len, stdin);
		
		// replace ending \n with \0
		for(i = 0; i < len; ++i) {
			if(command[i] == '\n') {
				command[i] = '\0';
				break;
			}
		}

		arg = strtok(command, DELIM);
		if(arg == NULL) { continue; }

		if(strcmp(arg, "write") == 0) {
			key = strtok(NULL, DELIM);
			value = strtok(NULL, DELIM);

			if(value == NULL) {
				printf("command error\n");
				continue;
			}

			hash_write(hash_tab, hash_size, key, value);
		} else if(strcmp(arg, "read") == 0) {
			key = strtok(NULL, DELIM);

			if(key == NULL) {
				printf("command error\n");
				continue;
			}


			hash_item * item = hash_read(hash_tab, hash_size, key);

			if(item != NULL) {
				printf("%s: %s\n", item->key, item->value);
			} else {
				printf("no item for key '%s'\n", key);
			}
		} else if (strcmp(arg, "delete") == 0) {
			key = strtok(NULL, DELIM);

			if(key == NULL) {
				printf("command error\n");
				continue;
			}

			bool succ = hash_delete(hash_tab, hash_size, key);

			if(succ) {
				printf("key '%s' deleted successfully\n", key);
			} else {
				printf("no item for key '%s'\n", key);
			}
		} else if (strcmp(arg, "dump") == 0) {
			hash_dump(hash_tab, hash_size);
		} else if (strcmp(arg, "quit") == 0) {
			break;
		} else {
			printf(
				"command '%s' not found\n"
				"usage:\n"
				"\twrite key value - write key and value to hash table\n"
				"\tread key - read value by key\n"
				"\tdelete key - delete by key\n"
				"\tdump - dump hash table\n"
				"\tquit - quit\n"
				, arg
			);
		}



	}

	return 0;
}
