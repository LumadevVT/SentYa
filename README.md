# SentYa

Este é o repositório do SentYa, o app de envio e recebimento de arquivos baseado no
[Sendme](https://github.com/n0-computer/sendme).

## Tecnologias Usadas

**Linguagem de Programação:** C++20.

**Interface de Usuário:** [Dear ImGui](https://github.com/ocornut/imgui)

**Janela, Seleção de Arquivos e Processos**: [SDL3](https://github.com/libsdl-org/SDL)

Esse projeto também depende do [Sendme](https://github.com/n0-computer/sendme), criado por [n0-computer](https://github.com/n0-computer).


## Rodando localmente

Antes de começar, certifique-se de que possui uma versão recente do CMake (v3.22+) e Git.
Verifique também se você possui um compilador com suporte decente a C++20.

Primeiramente, abra o terminal e rode o seguinte comando na pasta que deseja abrir o projeto:

```bash
  git clone --recursive https://github.com/LumadevVT/SentYa.git
```

Entre no diretório do projeto:

```bash
  cd SentYa
```

Crie a pasta onde o executável será criado:

```bash
  mkdir build
  cd build
```

Configure o projeto usando o comando:

```bash
  cmake .. -DCMAKE_BUILD_TYPE=Debug -G Ninja
```

Durante o desenvolvimento, usei o Ninja para fazer as builds do projeto, mas sinta-se à vontade para utilizar outro sistema, como:

- MinGW Makefiles (Windows);
- Visual Studio 17 2022 (Windows);
- Unix Makefiles (Linux);

Finalmente, para compilar o projeto, basta rodar:

```bash
  cmake --build .
```

## Aviso

Esse código tá bem bagunçado, sendo resultado de ter tentado terminar esse programa em menos de uma semana.
Eu poderei eventualmente fazer uma limpeza e reorganização desse código, mas por enquanto é o que tem pra hoje.

Outra coisa que quero avisar é que a acessibilidade do app é inexistente.
Isso acontece porque a biblioteca de UI que o projeto usa foi feita apenas para criar ferramentas para desenvolvedores de jogos.
Então, se você tem um colega que usa um leitor de tela ou algo parecido e não consegue usar o aplicativo, mil perdões.

Última coisa: eu sou brasileiro, mas gostaria que escrevesse o código e os commits em inglês.
Nada contra código em inglês, mas quero manter a consistência da base de código.

## Metas

- [ ] Refatorar, reorganizar e documentar o código.
- [ ] Fazer a aba "Tutorial" virar um Pop-Up.
    - A ideia é que ela apareça quando o usuário abra o aplicativo, exceto quando ele selecionar para não aparecer novamente.
- [ ] Adicionar suporte para outros idiomas, como o inglês.
- [ ] Automatizar o processo de criação de binários no github.
- [ ] Possivelmente builds para Mac?

## Licença

O código está coberto pela licença [MIT](https://choosealicense.com/licenses/mit/) e a licença [Apache 2.0](https://choosealicense.com/licenses/apache-2.0/), assim como o código original no repositório do Sendme.
