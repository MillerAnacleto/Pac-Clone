# [Pac Clone](https://github.com/MillerAnacleto/Pac-Clone)
Desenvolvido por:
- Miller Matheus Lima Anacleto Rocha - 13727954

## Como instalar

O jogo foi totalmente desenvolvido em C++, ele utiliza a biblioteca **SFML** que é utilizada para desenvolvimento rápido de aplicativos e jogos 2D, oferecendo módulos para gráficos, janelas, áudio e rede, ler [guia de uso](https://www.sfml-dev.org/index.php) para mais informações.. É conhecida pela simplicidade e adequada para iniciantes. As demais bibliotecas estão inclusas dentro do conjunto de bibliotecas padrão do C++.

### Como instalar SFML:

> É recomendado tentar executar o programa (exemplificado na seção **Como executar**) para verificar se **SFML** já está instalado. Um aviso do terminal em conjunto com instruções para instalá-la, específicas ao seus sistema, deve aparecer caso não esteja. Você pode também prosseguir com os comandos a seguir

Em sistemas baseados em Debian (como o Ubuntu):

```sh
sudo apt-get install libsfml-dev
```

## Como executar

Para rodar o jogo, deve-se executar as instruções :
```sh
make all # Compilar arquivos e gerar executável
make run # Executar jogo
```

## Como jogar

**Menu** é necessário apertar **enter** para iniciar o jogo <br>
**Jogo** durante o jogo a movimentação é feita a partir das teclas <kbd>↑</kbd> <kbd>↓</kbd> <kbd>→</kbd> <kbd>←</kbd> <br>
**Regras** as demais regras do jogo podem ser lidas no [Manual](https://www.nintendo.co.jp/clv/manuals/en/pdf/CLV-P-NABME.pdf)

## Descrições dos semáforos e threads utilizados

### Threads

No total, foram implementadas **quatro tipos de threads** para melhorar a eficiência e a responsividade do jogo. 

1. Thread de movimentação e ações do pacman
2. Uma thread para cada fantasma que controla sua movimentação, escolha de caminho e colisões
3. Uma thread que modifica o estado dos fantasmas, assim como seu target
4. Threads temporárias e texto na tela, como pontução após derrotar um fantasma

Todas elas foram feitas usando a biblioteca **threads** nativa do c++
Código exemplo de uso de thread desenvolvido em **Pac.hpp**:
```c
  #include <thread>

  thread* loopThread;

  loopThread = new thread(&Pac::loop, this);

  ~Pac(){
      if(loopThread->joinable())
          loopThread->join();
      
      delete loopThread;
      cout << "deleted P" << endl;
  }
```
### Semáforos

Os semáforos foram utilizados para controlar a pontuação do jogo, uma vez que 3 diferentes threads precisam  modificá-la.

1. A thread principal controla o jogo no geral, como Inicio de Jogo e Fim de Jogo
2. A thread do Pacman modifica o score quando consome uma esfera amarela
3. As threads do fantasmas modificam o score quando entram em contato com o pacman e estão em modo *Frightned*

Essa ação é realizada pela biblioteca **Mutex**, também nativa do c++ : 

Código exemplo dos usos de **Mutex** desenvolvido em **ControlGame.hpp** e **Pac.hpp**::
```c++
// em ControlGame.hpp
class ControlGame{
    public:
        static mutex scoreMutex;
        static int score;
};

// em Pac.hpp
lock_guard<mutex> lock(ControlGame::scoreMutex);
ControlGame::score += addScore;
ControlGame::scoreMutex.unlock();

```

Dessa forma, condições de corrida, antes presentes e que poderiam ocasionar glitches na pontuação, são mitigadas pelo uso dos semáforos. 

## Vídeo de apresentação

sem link

## Imagens do jogo

### Menu
![Menu](images/Menu.png)

### Play
![Play](images/Play.png)

### GameOver
![GameOver](images/GameOver.png)
