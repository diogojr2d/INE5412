// Author: Diogo Junior de Souza

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADD_FUNC 1
#define REMOVE_FUNC 2
#define CALC_AVERAGE 3
#define EXPORT_DB 4
#define COMPACT_DB 5

typedef struct {
    unsigned int id;
    char nome[256];
    char sexo;
    float salario;
} Funcionario;

FILE* database;

void showHelp() {
    printf("\nO programa deve receber dois parâmetros, o nome do arquivo da base de dados e a operação desejada.\n");
    printf("Exemplo: ./funcionarios basededados 2\n\n");
    printf("Operações:\n");
    printf("1 - Inserir funcionário\n");
    printf("2 - Remover funcionário\n");
    printf("3 - Calcular média salarial\n");
    printf("4 - Exportar base de dados para arquivo de texto\n");
    printf("5 - Compactar base de dados\n\n");
}

void addFuncionario() {
    Funcionario func;
    
    printf("Adicionando novo funcionário...\n");
    printf("Digite o id: ");
    int id;
    scanf("%d", &id);
    if (id > 0) {
        // verifica se o id já existe
        while (1) {
            Funcionario tmp;
            fread(&tmp, sizeof(Funcionario), 1, database);
            if (feof(database) != 0) {
                func.id = id;
                break;
            }
            if (id == tmp.id) {
                printf("Este id já existe.\n");
                return;
            }
        }
    } else {
        printf("Id deve ser um numero inteiro maior que zero.\n");
        return;
    }

    printf("Digite o nome: ");
    scanf("%s", &func.nome);
    char c;
    while((c = getchar()) != '\n' && c != EOF) {
        /* discard extra characters */
    }
    printf("Digite o gênero (m or f): ");
    scanf("%s", &func.sexo);
    while ((func.sexo != 'm') && (func.sexo != 'f')) {
        while((c = getchar()) != '\n' && c != EOF) {
            /* discard extra characters */
        }
        printf("Gênero deve ser 'm' or 'f': ");
        scanf("%s", &func.sexo);
    }
    printf("Digite o salário: ");
    scanf("%f", &func.salario);
    fwrite(&func, sizeof(Funcionario), 1, database);
}

void removeFuncionario() {
    printf("Removendo funcionario...\n");
    printf("Digite o id: ");
    int id;
    scanf("%d", &id);
    if (id <= 0) {
        printf("Id deve ser um numero inteiro maior que zero.\n");
        return;
    }
    while (1) {
        Funcionario tmp;
        fread(&tmp, sizeof(Funcionario), 1, database);
        if (feof(database) != 0) {
            printf("Funcionario com este id não encontrado.\n");
            return;
        }
        if (tmp.id == id) {
            fseek(database, -sizeof(Funcionario), SEEK_CUR);
            tmp.id = 0;
            fwrite(&tmp, sizeof(Funcionario), 1, database);
            printf("Funcionario removido com sucesso.\n");
            return;
        }
    }
}

void averageSalary() {
    printf("Calculando média salarial, por gênero...\n");
    int nMales = 0;
    int nFemales = 0;
    float sumMale = 0;
    float sumFemale = 0;
    while (1) {
        Funcionario tmp;
        fread(&tmp, sizeof(Funcionario), 1, database);
        if (feof(database) != 0)
            break;
        if (tmp.id == 0)
            continue;
        if (tmp.sexo == 'm') {
            nMales++;
            sumMale += tmp.salario;
        } else {
            nFemales++;
            sumFemale += tmp.salario;
        }
    }
    printf("Salário médio do sexo masculino: %f \n", sumMale/nMales);
    printf("Salário médio do sexo feminino: %f \n", sumFemale/nFemales);
}

void exportDatabase(char* orig) {
    printf("Exportando Base de Dados para arquivo de texto...\n");
    char filename[256];
    printf("Digite o nome do arquivo de saída onde exportar os dados: ");
    scanf("%s)", &filename);
    printf("orig: %s - filename: %s -\n", orig, filename);
    if (strcmp(orig, filename) == 0) {
        printf("O nome do arquivo de saída deve ser diferente do arquivo original da base de dados.\n");
        return;
    }
    FILE *final;
    if ((final = fopen(filename, "w")) == NULL) {
        printf("Erro ao criar arquivo de texto.\n");
        return;
    } else {
        while (1) {
            Funcionario tmp;
            fread(&tmp, sizeof(Funcionario), 1, database);
            if (feof(database) != 0)
                break;
            if (tmp.id != 0) {
                fprintf(final, "ID: %d \n", tmp.id);
                fprintf(final, "Name: %s \n", tmp.nome);
                fprintf(final, "Gender: %c \n", tmp.sexo);
                fprintf(final, "Salary: %f \n", tmp.salario);
            }
        }
        fclose(final);
        printf("Base de dados exportada.\n");
    }
}

void compactDatabase(char* filename) {
    printf("Compactando base de dados...\n");
    FILE* newDatabase;
    if ((newDatabase = fopen("newDatabase", "wb")) == NULL) {
        printf("Erro ao criar arquivo de cópia do base de dados.\n");
    } else {
        while (1) {
            Funcionario tmp;
            fread(&tmp, sizeof(Funcionario), 1, database);
            if (feof(database) != 0)
                break;
            if (tmp.id != 0) {
                fwrite(&tmp, sizeof(Funcionario), 1, newDatabase);
            }
        }
        fclose(newDatabase);
        remove(filename);
        rename("newDatabase", filename);
    }
    printf("Base de dados compactada.\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        showHelp();
    } else {
        char* filename = argv[1];
        int option = atoi(argv[2]);
        // Cria o arquivo caso esse não exista.
        database = fopen(argv[1], "ab+");
        fclose(database);

        switch(option) {
            case ADD_FUNC:
                if ((database = fopen(filename, "ab+")) == NULL) {
                    printf("Erro ao abrir ou criar o arquivo da base de dados.\n");
                    return 1;
                }
                addFuncionario();
                break;
            
            case REMOVE_FUNC:
                if ((database = fopen(filename, "rb+")) == NULL) {
                    printf("Erro ao abrir o arquivo da base de dados.\n");
                    return 1;
                }
                removeFuncionario();
                break;

            case CALC_AVERAGE:
                if ((database = fopen(filename, "rb")) == NULL) {
                    printf("Erro ao abrir o arquivo da base de dados.\n");
                    return 1;
                }
                averageSalary();
                break;

            case EXPORT_DB:
                if ((database = fopen(filename, "rb")) == NULL) {
                    printf("Erro ao abrir o arquivo da base de dados.\n");
                    return 1;
                }
                exportDatabase(filename);
                break;

            case COMPACT_DB:
                if ((database = fopen(filename, "rb")) == NULL) {
                    printf("Erro ao abrir o arquivo da base de dados.\n");
                    return 1;
                }
                compactDatabase(filename);
                break;
            
            default:
                printf("Operação incorreta.\n");
                showHelp();
        }

        if (database != NULL) {
            fclose(database);
        }
    }

    printf("Fim do programa.\n");
    return 0;
}