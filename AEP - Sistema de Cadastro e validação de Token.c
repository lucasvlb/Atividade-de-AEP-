#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

void criptografar(char *str, char chave) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] ^= chave;
    }
}

int gerar_token() {
    srand(time(NULL));
    return rand() % 9000 + 1000;
}

void registrar() {
    char usuario[100], senha[100],chave = 0xAA;
    int telefone[20];

    FILE *arquivo = fopen("usuarios.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("Digite o nome de usuario: ");
    scanf("%s", usuario);
    
    printf("Digite a senha: ");
    scanf("%s", senha);
    
    printf("Digite o n�mero de telefone: ");
    scanf("%s", telefone);

    criptografar(senha, chave);
    
    fprintf(arquivo, "%s %s %s\n", usuario, senha, telefone);
    fclose(arquivo);

    printf("Usu�rio registrado com sucesso!\n");
}

void login() {
    char usuario[100], senha[100], telefone[20];
    char nomeUsuario[100], senhaCriptografada[100], telefoneRegistrado[20];
    char chave = 0xAA;
    int usuarioEncontrado = 0;

    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("Digite o nome de usuario: ");
    scanf("%s", usuario);
    
    printf("Digite a senha: ");
    scanf("%s", senha);

    while (fscanf(arquivo, "%s %s %s", nomeUsuario, senhaCriptografada, telefoneRegistrado) != EOF) {
        criptografar(senhaCriptografada, chave);
        
        if (strcmp(usuario, nomeUsuario) == 0 && strcmp(senha, senhaCriptografada) == 0) {
            usuarioEncontrado = 1;
            
            int token = gerar_token();
            printf("Token de verifica��o enviado para o telefone %s: %d\n", telefoneRegistrado, token);  // Simula o envio de SMS ao usu�rio
            
            int tokenInserido;
            printf("Digite o token recebido no seu celular: ");
            scanf("%d", &tokenInserido);

            if (token == tokenInserido) {
                printf("Login bem-sucedido!\n");
            } else {
                printf("Token incorreto! Login falhou.\n");
            }
            break;
        }
    }

    if (!usuarioEncontrado) {
        printf("Usu�rio ou senha inv�lidos!\n");
    }

    fclose(arquivo);
}
void alterar_usuario() {
    char usuario_alterar[100];
    char novoUsuario[100], novaSenha[100];
    char usuario[100], senha[100];
    char chave = 0xAA;
    int usuarioEncontrado = 0;

    printf("Digite o nome do usu�rio que deseja alterar: ");
    scanf("%s", usuario_alterar);

    FILE *arquivo = fopen("usuarios.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    while (fscanf(arquivo, "%s %s", usuario, senha) != EOF) {
        if (strcmp(usuario, usuario_alterar) == 0) {
            printf("Digite o novo nome de usu�rio: ");
            scanf("%s", novoUsuario);

            printf("Digite a nova senha: ");
            scanf("%s", novaSenha);

            criptografar(novaSenha, chave); 
           	
			fprintf(temp, "%s %s\n", novoUsuario, novaSenha);
            usuarioEncontrado = 1;
        } else {
            fprintf(temp, "%s %s\n", usuario, senha);
        }
    }

    fclose(arquivo);
    fclose(temp);
    
    if (usuarioEncontrado) {
        remove("usuarios.txt");
        rename("temp.txt", "usuarios.txt");
        printf("Usu�rio '%s' alterado com sucesso!\n", usuario_alterar);
    } else {
        remove("temp.txt");  
        printf("Usu�rio n�o encontrado!\n");
    }
}

void exibir_usuarios() {
    char usuario[100], senha[100];
    int usuariosEncontrados = 0;

    FILE *arquivo = fopen("usuarios.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    
    while (fscanf(arquivo, "%s %s", usuario, senha) != EOF) {
        if (usuariosEncontrados == 0) {
            printf("Lista de usu�rios cadastrados:\n\n");
        }
        printf("- %s\n", usuario);
        usuariosEncontrados = 1; 
    }
    if (!usuariosEncontrados) {
        printf("\nLista de usu�rios vazia.\n");
    }

    fclose(arquivo);
}

void excluir_usuario() {
    char usuarioParaExcluir[100];
    char usuario[100], senha[100];
    int usuarioEncontrado = 0;

    printf("Digite o nome do usu�rio que deseja excluir: ");
    scanf("%s", usuarioParaExcluir);

    FILE *arquivo = fopen("usuarios.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (arquivo == NULL || temp == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    while (fscanf(arquivo, "%s %s", usuario, senha) != EOF) {
        if (strcmp(usuario, usuarioParaExcluir) != 0) {
            fprintf(temp, "%s %s\n", usuario, senha);
        } else {
            usuarioEncontrado = 1;
        }
    }

    fclose(arquivo);
    fclose(temp);

    
    if (usuarioEncontrado) {
        remove("usuarios.txt");
        rename("temp.txt", "usuarios.txt");
        printf("Usu�rio '%s' exclu�do com sucesso!\n", usuarioParaExcluir);
    } else {
        remove("temp.txt");
        printf("Usu�rio n�o encontrado!\n");
    }
}
int main() {
	setlocale(LC_ALL, "Portuguese");
    int opcao;

    while (1) {
        printf("\nSistema de Registro e Login\n");
        printf("1. Registrar\n");
        printf("2. Login\n");
        printf("3. Alterar Usu�rio\n");
        printf("4. Exibir todos os usu�rios\n");
        printf("5. Excluir um usu�rio\n");
        printf("6. Sair\n");
        printf("Escolha uma op��o: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                registrar();
                break;
            case 2:
                login();
                break;
            case 3:
                alterar_usuario();
                break;    
            case 4:
                exibir_usuarios();
                break;
            case 5:
                excluir_usuario();
                break;
            case 6:
                printf("Saindo...\n");
                return 0;    
            default:
                printf("Op��o inv�lida!\n");
                break;
        }
    }

    return 0;
}
