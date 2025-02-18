# Tarefa_aula_10-02
Repositório criado para versionamento da tarefa do dia 10 de Fevereiro 

## Compilação e Execução

1. Certifique-se de que o SDK do Raspberry Pi Pico está configurado no seu ambiente.
2. Compile o programa utilizando a extensão **Raspberry Pi Pico Project** no VS Code:
   - Abra o projeto no VS Code, na pasta Tarefa_ADC tem os arquivos necessários para importar 
   o projeto com a extensão **Raspberry Pi Pico Project**.
   - Vá até a extensão do **Raspberry pi pico project** e após importar os projetos  clique em **Compile Project**.
3. Coloque a placa em modo BOOTSEL e copie o arquivo `Tarefa_ADC.uf2`  que está na pasta build, para a BitDogLab conectado via USB.


**OBS1: Devem importar os projetos para gerar a pasta build, pois a mesma não foi inserida no repositório**
**OBS2: Em algumas versões da BitDogLab o posicionamento dos eixos X e Y podem estar invertidos**

## Emulação com Wokwi

Para testar os programas sem hardware físico, você pode utilizar o **Wokwi** para emulação no VS Code:

1. Instale a extensão **Wokwi for VS Code**
3. Inicie a emulação:
   - Clique no arquivo `diagram.json` e inicie a emulação.
4. Teste o funcionamento do programa diretamente no ambiente emulado.
   
**OBS: Os arquivos diagram.json e wokwi.toml foram inseridos para cada atividade.**

## Link com demonstração no youtube

Demonstração do funcionamento do projeto na BitDogLab


## Colaboradores
- [PauloCesar53 - Paulo César de Jesus Di Lauro ] (https://github.com/PauloCesar53)
