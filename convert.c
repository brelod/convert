#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASCII 0
#define BIN 2
#define DEC 10
#define HEX 16

typedef struct {
        int src;
        int dst;
        int start;
        int end;
} config_t;

void print(int num, int fmt)
{
        switch(fmt) {
                case BIN:
                        for (int i=7; i>=0; --i)
                                putchar(num & 1<<i ? '1' : '0');
                        break;

                case DEC: printf("%d", num); break;
                case HEX: printf("0x%02x", num); break;
                case ASCII: printf("%c", num); break;
                default: printf("Format %d is not implemented", fmt);
        }
}

int str2int(const char *str, int base)
{
        int add = 0;
        int place = 1;
        int result = 0;

        for (int i=strlen(str)-1; i>=0; --i) {
                if (str[i] >= 0x30 && str[i] < 0x40)      // 0-9
                        add = str[i] - 0x30;
                else if (str[i] >= 0x41 && str[i] < 0x5B) // A-F
                        add = str[i] - 0x37;
                else if (str[i] >= 0x61 && str[i] < 0x7B) // a-f
                        add = str[i] - 0x57;
                else
                        return -1;

                add *= place;

                if ((1U<<31)-1 - result < add) // int overflow
                        return -1;

                result += add;
                place *= base;
        }
        return result;
}

int parse_type(const char *str)
{
        if (strcmp(str, "ascii") == 0)
                return ASCII;
        if (strcmp(str, "bin") == 0)
                return BIN;
        if (strcmp(str, "dec") == 0)
                return DEC;
        if (strcmp(str, "hex") == 0)
                return HEX;
        return -1;
}

int argparse(config_t *config, int argc, char **argv)
{
        int i, c;
        char *key, *val;

        for (i=1; i<argc; ++i) {
                key = argv[i];
                if (strcmp(key, "--help") == 0 || strcmp(key, "-h") == 0) {
                        printf("Usage: %s [options]\n"
                                "--src=<ascii|bin|dec|hex>      specify source number format\n"
                                "--dst=<ascii|bin|dec|hex>      specify destination number format\n"
                                "--start=<decimal-number>       generate source number >= to this\n"
                                "--end=<decimal-number>         generate destination number < to this\n",
                                argv[0]
                        );
                        return -1;
                }

                for (c=0; argv[i][c] != '=';  ++c) {
                        if (argv[i][c] == '\0') {
                                val = NULL;
                                goto invalid_arg;
                        }
                }
                argv[i][c] = '\0';
                ++c;
                val = &argv[i][c];

                if (strcmp(key, "--src") == 0) {
                        if ((config->src = parse_type(val)) < 0)
                                goto invalid_arg;
                }

                else if (strcmp(key, "--dst") == 0) {
                        if ((config->dst = parse_type(val)) < 0)
                                goto invalid_arg;
                }

                else if (strcmp(key, "--start") == 0) {
                        if ((config->start = str2int(val, 10)) < 0)
                                goto invalid_arg;
                        if (config->start > 255)
                                goto invalid_arg;
                }

                else if (strcmp(key, "--end") == 0) {
                        if ((config->end = str2int(val, 10)) < 0)
                                goto invalid_arg;
                        if (config->end > 256)
                                goto invalid_arg;
                }

                else
                        goto invalid_arg;

                if (config->start >= config->end)
                        goto invalid_arg;
        }
        return 0;

invalid_arg:
        printf("Invalid argument: %s=%s\n", key, val);
        return -1;
}

int main(int argc, char **argv)
{
        int src, dst;
        config_t config;
        char buffer[10];

        // Default values
        config.src = BIN;
        config.dst = HEX;
        config.start = 0;
        config.end = 256;

        if (argparse(&config, argc, argv) != 0)
                return 1;

        printf("config.src = %d\n", config.src);
        printf("config.dst = %d\n", config.dst);
        printf("config.start = %d\n", config.start);
        printf("config.end = %d\n", config.end);

        while (1) {
                srand(time(0));
                src = rand() % (config.end - config.start) + config.start;

                print(src, config.src);
                putchar(':');

                if (fgets(buffer, sizeof(buffer), stdin) == NULL)
                        break;
                buffer[strlen(buffer)-1] = '\0';  // Remove \n
                dst = config.dst ? str2int(buffer, config.dst) : buffer[0];

                if (src != dst) {
                        print(dst, config.dst);
                        printf(" is ivalid. Expected:");
                        print(src, config.dst);
                        putchar('\n');
                } else
                        printf("OK\n");
        }

        return 0;
}
