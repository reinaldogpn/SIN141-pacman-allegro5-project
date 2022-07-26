Pac-Man Allegro 5 / C++
#

Um projeto desenvolvido para a disciplina de Computação Orientada a Objetos (SIN 141) do curso de Sistemas de Informação na Universidade Federal de Viçosa (UFV/CRP).

**********************************************************************************

Projeto implementado por: Reinaldo Gonçalves Pereira Neto | Matrícula: 7627
	
**********************************************************************************

** ESTRATÉGIA DE PERSEGUIÇÃO **

Esse estilo de movimento é utilizado pelos fantasmas Blinky (vermelho) e Pinky (rosa).

Consiste em um algoritmo básico de perseguição tal que:

Considerando as coordenadas (x, y) do fantasma F e as coordenadas (x, y) do player P, fica bem fácil definir que se x(F) é MENOR que x(P), F está à esquerda de P;
Nesse caso, x(F) será incrementado.

Caso contrário, se x(F) é MAIOR que x(P), F está à direita de P;
Logo, x(F) será decrementado.

Da mesma forma, se y(F) é MENOR que y(P), definimos que F está acima de P;
Nesse caso, y(F) será incrementado.

Caso contrário, se y(F) é MAIOR que y(P), F está abaixo de P;
Logo, y(F) será decrementado.

Tendo em mente essa ideia, são aplicados ao algoritmo os testes de colisão com os obstáculos (paredes) do mapa, e assim, são necessários alguns cálculos.

Por exemplo, caso o jogo esteja em uma situação onde x(F) é menor que x(P), o fantasma F receberá a intenção de movimento para à DIREITA, mas x(F) não é incrementado ainda, pois
antes disso, o algoritmo testa se há colisão do fantasma F com um obstáculo na sua próxima posição x.

Caso NÃO haja colisão, então x é incrementado e o fantasma F se move para a direita.

Caso contrário, o algoritmo detecta uma colisão e isso fará com que o fantasma F receba a intenção de desviar-se do obstáculo, se movendo para CIMA ou para BAIXO (nunca para à esquerda,
pois assim ele estaria se movendo na direção contrária do player P, o que não faz sentido).

Para decidir se irá se mover para cima ou para baixo, são realizados cálculos de distância entre o fantasma F e o player P. 
Para isso, eu implementei o método "double calcDistance(int Xa, int Ya, int Xb, int Yb)" na classe "Character".
Este método recebe as coordenadas (x, y) dos dois personagens e aplica a fórmula para cálculo de distância entre dois pontos no plano cartesiano, 
retornando assim o valor absoluto(módulo) do resultado para a variável (double) "dist_atual". A fórmula está descrita abaixo:

abs(sqrt(pow(Xb-Xa, 2)+pow(Yb-Ya, 2))) | Xa, Ya são as coordenadas do fantasma F, Xb, Yb são as coordenadas do player P.

Sabendo da distância atual entre os dois personagens, então o algoritmo irá calcular a distância caso ele se mova para CIMA ou para BAIXO, como mencionado anteriormente.
Então, por exemplo, se a distância entre os personagens caso o fantasma F se mova para CIMA for menor que a distância caso ele se mova para BAIXO, o fantasma F tende a se
locomover para cima. O algoritmo sempre checa se não existe um obstáculo na próxima posição do personagem antes do movimento ser realizado, impossibilitando assim a transposição das paredes.

*****

Vale mencionar também que os fantasmas Blinky e Pinky tendem a alternar seu estilo de movimento entre "perseguir" e "aleatório" a cada X tempo passado.

Isso é implementado na função principal do programa, através do método "(al_get_timer_count(timer)%150 == 0)" dentro do loop do jogo.

A função do Allegro "al_get_timer_count()" retorna o valor da contagem do timer (quantos "ticks" se passaram até o momento), e como meu TIMER foi criado com o valor (1.0 / 15.0),
o timer conta um "tick" a cada aproximadamente 0,07 segundo.

Concluindo, então cada vez que a divisão da quantidade de "ticks" passados por 150 for exata (o que resulta em aproximadamente 10 segundos), os movimentos dos dois fantasmas serão alternados.
	
**********************************************************************************
	
Ficam meus agradecimentos ao prof. Guilherme Pena pela oportunidade de implementar esse projeto, pois através dele, aprendi MUITO sobre C++ e também sobre a biblioteca Allegro 5.
	
**********************************************************************************
