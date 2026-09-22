#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void xorEncryptDecrypt(char* data, size_t dataLen, const char* key) {
    size_t keyLen = strlen(key);

    for (size_t i = 0; i < dataLen; ++i) {
        data[i] ^= key[i % keyLen];
    }
}

int main() {
    char choice;
    printf("Enter 'e' to encrypt or 'd' to decrypt: ");
    scanf(" %c", &choice);
    getchar();

    if (choice == 'e') {
        char plaintext[1000];
        char filename[50];
        char key[20];

        printf("Enter the plaintext: ");
        fgets(plaintext, sizeof(plaintext), stdin);

        /*
         * If the plaintext is longer than 999 characters,
         * fgets() leaves the remainder in stdin. This removes it.
         */
        if (strchr(plaintext, '\n') == NULL) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }

        plaintext[strcspn(plaintext, "\n")] = '\0';

        printf("Enter the filename to save encrypted data: ");
        scanf("%49s", filename);

        printf("Enter the key: ");
        scanf("%19s", key);

        size_t plaintextLen = strlen(plaintext);

        FILE* file = fopen(filename, "wb");
        if (file == NULL) {
            printf("Error opening file for writing.\n");
            return 1;
        }

        xorEncryptDecrypt(plaintext, plaintextLen, key);

        /*
         * Use plaintextLen, not strlen(plaintext), because the
         * encrypted data may contain '\0' bytes.
         */
        fwrite(plaintext, sizeof(char), plaintextLen, file);

        fclose(file);
    }
    else if (choice == 'd') {
        char filename[50];
        char key[20];

        printf("Enter the filename to read encrypted data: ");
        scanf("%49s", filename);

        printf("Enter the key: ");
        scanf("%19s", key);

        FILE* file = fopen(filename, "rb");
        if (file == NULL) {
            printf("Error opening file for reading.\n");
            return 1;
        }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* encryptedData = malloc(fileSize + 1);
        if (encryptedData == NULL) {
            printf("Memory allocation failed.\n");
            fclose(file);
            return 1;
        }

        fread(encryptedData, sizeof(char), fileSize, file);
        fclose(file);

        /*
         * Decrypt exactly fileSize bytes, including any '\0'
         * bytes that occur in the encrypted data.
         */
        xorEncryptDecrypt(encryptedData, fileSize, key);

        /*
         * Add the string terminator AFTER decryption.
         */
        encryptedData[fileSize] = '\0';

        printf("Decrypted data: %s\n", encryptedData);

        free(encryptedData);
    }
    else {
        printf("Invalid choice.\n");
    }

    return 0;
}
