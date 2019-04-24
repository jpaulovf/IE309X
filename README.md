# IE309X
## Repositório do projeto da disciplina IE309X - Oficina de IoT

---

### 1. Resumo

Este repositório contém os códigos desenvolvidos para a disciplina IE309X da Unicamp, ministrada durante o primeiro semestre de 2019. O grupo deste trabalho é composto pelos seguintes membros:

- Fabio Bassan
- Felipe Marques
- Gabriel Legramanti
- João Fracarolli

Neste repositório colocaremos o código-fonte do ESP32 e outros eventuais códigos que forem necessários, além de outros arquivos do projeto.

### 2. Clonando

Para clonar este repositório, digite:

`git clone https://github.com/jpaulovf/IE309X.git`

### 3. Conteúdo

Este repositório, até o momento, possui os seguintes diretórios:

- **Source**: Diretório com os códigos-fonte.
    - **TesteBlynk**: Diretório com o primeiro teste do ESP32 com Blynk;
    - **libraries**: Bibliotecas necessárias para que os testes funcionem.
    - **ESP32_SPI**: Teste do ESP32 com comunicação SPI
    - **EnergyMeter**: Arquivos com o firmware principal

## Detalhamento

### 1. Introdução

Diante de um cenário onde há necessidade imediata de aumento da eficiência energética e controle de emissão de poluentes, novas tecnologias surgem visando atingir essas demandas. Um destes conceitos é o de “cidades inteligentes”, que também busca melhorar a qualidade de vida e bem-estar da população através das tecnologias.
Diante disso, o interesse no acesso às informações em tempo real permitem a integração de sensores e dispositivos num sistema maior, proporcionando maior grau de controle ao operador. Um dispositivo com grande evolução e que permite o acesso a um grande número de informações é o smartphone. Com ele, podemos verificar o clima, notícias, o número de batimentos cardíacos do usuário, aceleração, GPS e outras diversas funções e informações.
Numa residência, por exemplo, é usual que tenhamos uma vez ao mês uma conta de energia elétrica informando dados de consumo, o valor do kWh, média de consumo e acompanhamento dos últimos meses. Logo, para aqueles que gostariam de informações mais detalhadas e em tempo real, isso não é fornecido por parte das concessionárias de energia. 

### 2. Objetivo

O projeto visa uma solução que permite ao usuário de energia elétrica acesso às informações de consumo em tempo real através de um aplicativo instalado no smartphone, bastando para isso que o dispositivo tenha uma conexão wi-fi. O consumo pode ser medido e informado, possibilitando que o consumidor seja melhor informado e consiga ser um membro mais ativo do seu próprio consumo. Além disso, o usuário pode ser avisado quando o seu consumo de energia ultrapassar um limiar estabelecido, podendo então atuar para diminuir o gasto de energia imediatamente.
Para isso, propomos um hardware composto de um circuito integrado especializado para a medição de parâmetros de consumo de energia, o [ADE7753](https://www.analog.com/media/en/technical-documentation/data-sheets/ADE7753.pdf) (monofásico) ou o [ADE7758](https://www.analog.com/media/en/technical-documentation/data-sheets/ade7758.pdf) (trifásico); um microprocessador especializado para aplicações IoT, o [ESP32](https://www.espressif.com/en/products/hardware/esp32/overview) da Espressif e outros componentes eletrônicos diversos, como resistores, capacitores, reguladores de tensão e conectores. Inicialmente pretende-se que a programação do microcontrolador seja feita usando as bibliotecas mais simples o possível, como [MicroPython](https://micropython.org/) ou a própria [IDE do Arduino](https://www.arduino.cc/en/Main/Software) e que os dados medidos sejam disponibilizados para o usuário através da plataforma [Blynk](https://blynk.io/). Nesta plataforma também pretende-se usar alarmes para avisar ao usuário sobre alto consumo de energia.

### 3. Lista de materiais e orçamento estimado

| Componente                | Quantidade | Custo (R$) |
|---------------------------|------------|------------|
| ESP32 Wroom               |      1     |      62,00 |
| ADE7758                   |      1     |      30,00 |
| Transformador de corrente |      3     |      45,00 |
| LM7805                    |      1     |       0,95 |
| Resistores diversos       |     15     |       2,50 |
| Capacitores diversos      |     10     |       0,50 |
| Fonte AC/DC 12V, 3W       |      1     |      20,00 |
| **TOTAL**                 |            |     160,95 |

Fonte: [Baú da Eletrônica](http://www.baudaeletronica.com.br)

### 4. Cronograma

| Atividade             | Março       | Abril       | Maio        | Junho       |
|-----------------------|-------------|-------------|-------------|-------------|
|                       | S1&nbsp;S2&nbsp;S3&nbsp;S4 | S1&nbsp;S2&nbsp;S3&nbsp;S4 | S1&nbsp;S2&nbsp;S3&nbsp;S4 | S1&nbsp;S2&nbsp;S3&nbsp;S4 |
| Estudo preliminar     | X&nbsp; &nbsp;X&nbsp; &nbsp;X&nbsp; &nbsp;X&nbsp; &nbsp;|             |             |             |
| Hardware: esquemático |             | X&nbsp; &nbsp;X&nbsp; &nbsp;X&nbsp; &nbsp;#&nbsp; &nbsp;| #&nbsp; &nbsp;#&nbsp; &nbsp;#&nbsp; &nbsp;#&nbsp; &nbsp;|             |
| Hardware: PCB         |             | X&nbsp; &nbsp;X&nbsp; &nbsp;X&nbsp; &nbsp;#&nbsp; &nbsp;| #&nbsp; &nbsp;#&nbsp; &nbsp;#&nbsp; &nbsp;#&nbsp; &nbsp;|             |
| Firmware              |             | X&nbsp; &nbsp;X&nbsp; &nbsp;X&nbsp; &nbsp;#&nbsp; &nbsp;| #&nbsp; &nbsp;#&nbsp; &nbsp;#&nbsp; &nbsp;#&nbsp; &nbsp;|             |
| Aplicação             |             | X&nbsp; &nbsp;X&nbsp; &nbsp;X&nbsp; &nbsp;#&nbsp; &nbsp;| #&nbsp; &nbsp;#&nbsp; &nbsp;#&nbsp; &nbsp;#&nbsp; &nbsp;|             |
| Integração            |             |             |&nbsp;&nbsp; &nbsp;&nbsp; &nbsp;&nbsp; &nbsp; #&nbsp; &nbsp;#&nbsp; &nbsp;| #&nbsp; &nbsp;#&nbsp; &nbsp;      |
| Testes unitários      |             | X&nbsp; &nbsp;X&nbsp; &nbsp;X&nbsp; &nbsp;#&nbsp; &nbsp;| #&nbsp; &nbsp;#&nbsp; &nbsp;#&nbsp; &nbsp;#&nbsp; &nbsp;| #&nbsp; &nbsp;#&nbsp; &nbsp;#&nbsp; &nbsp;#&nbsp; &nbsp;|
| Testes finais         |             |             |             | #&nbsp; &nbsp;#&nbsp; &nbsp;#&nbsp; &nbsp;#&nbsp; &nbsp;|

---

\#: Planejado

X : Executado

--- 
*Última atualização* : 24/04/2019 por [jpaulovf](http://github.com/jpaulovf)
