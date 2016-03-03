#include <criterion/criterion.h>
#include "../savesplit.h"
#include <strings.h>
#include <unistd.h>
#include <stdio.h>

#define BUFSIZE 64
const unsigned int inc = BUFSIZE;
char *pasta;
int pipelink[2] = { 1 };

void setup(void) {
    pasta = malloc(inc); 
    pipe(pipelink);
}

void teardown(void) {
    free(pasta); 
    close(pipelink[0]); close(pipelink[1]);
}

Test(default, buffering, .init = setup, .fini = teardown) {
    char *text1;
    text1 = "Hello, how are you?";

    write(pipelink[1], text1, strlen(text1));
    buffering(pipelink[0], &pasta, inc);
    cr_assert(strcmp(text1, pasta) == 0, "Buffering error!");

    teardown(); setup();

    text1 = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi quis tempus lectus, vel ornare nibh. Donec vitae orci imperdiet, tempor lectus posuere, tempus urna. Vestibulum a tincidunt orci. Aenean quis bibendum leo. Sed imperdiet nunc eget condimentum venenatis. Vestibulum quis elit at mi ultrices aliquam. Vestibulum lacinia non est ut convallis. Etiam vitae tellus ac nisi aliquam efficitur. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Integer non blandit nulla, ut molestie libero. Etiam ultricies nisi felis, quis dignissim massa lacinia in. Sed tristique lectus nunc, id tempor tortor interdum eu. Curabitur vitae euismod turpis, rutrum bibendum enim. Maecenas ultrices lobortis dignissim. Donec ultrices maximus libero, nec lacinia nisi scelerisque nec.";
    write(pipelink[1], text1, strlen(text1));
    buffering(pipelink[0], &pasta, inc);

    cr_expect(strcmp(text1, pasta) == 0, "Long buffering error!");
    
}
