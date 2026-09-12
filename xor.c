#include <stdio.h>
#include <string.h>

void xorEncryptDecrypt(char *data, const char *key) {
    int keyLen = strlen(key);
    for (int i = 0; data[i] != '\0'; ++i) {
        data[i] ^= key[i % keyLen];
    }
}

int main() {
    char choice;
    printf("Enter 'e' to encrypt or 'd' to decrypt: ");
    scanf("%c", &choice);

    if (choice == 'e') {
        char plaintext[1000];
        char filename[50];
        char key[20];

        printf("Enter the plaintext: ");
        fgets(plaintext, sizeof(plaintext), stdin);
        plaintext[strcspn(plaintext, "\n")] = '\0'; // Remove newline

        printf("Enter the filename to save encrypted data: ");
        scanf("%s", filename);

        printf("Enter the key: ");
        scanf("%s", key);

        FILE *file = fopen(filename, "wb");
        if (file == NULL) {
            printf("Error opening file for writing.\n");
            return 1;
        }

        xorEncryptDecrypt(plaintext, key);
        fwrite(plaintext, sizeof(char), strlen(plaintext), file);
        fclose(file);
    } else if (choice == 'd') {
        char filename[50];
        char key[20];

        printf("Enter the filename to read encrypted data: ");
        scanf("%s", filename);

        printf("Enter the key: ");
        scanf("%s", key);

        FILE *file = fopen(filename, "rb");
        if (file == NULL) {
            printf("Error opening file for reading.\n");
            return 1;
        }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        char *encryptedData = malloc(fileSize + 1);
        if (encryptedData == NULL) {
            printf("Memory allocation failed.\n");
            fclose(file);
            return 1;
        }

        fread(encryptedData, sizeof(char), fileSize, file);
        encryptedData[fileSize] = '\0';
        fclose(file);

        xorEncryptDecrypt(encryptedData, key);
        printf("Decrypted data: %s\n", encryptedData);

        free(encryptedData);
    } else {
        printf("Invalid choice.\n");
    }

    return 0;
}