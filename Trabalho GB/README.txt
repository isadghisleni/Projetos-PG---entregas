Arquivo README referente ao trabalho de Grau C da disciplina de Processamento Gráfico

Trabalho de implementação de um aplicativo simples para criar efeitos em imagens e video.

O arquivo possui documentos de biblioteca e imagens anexadas, essenciais para o funcionamento do programa.
Programa desenvolvido em Visual Studio 2022, versão 17.1.1 (64bits), utilizando a linguagem C++.

Ao rodar o programa, o usuário deve escolher entre 3 opções:

1 - Escolha o nome dos ossos
2 - Filtros
3 - Webcam 

cada opção contém:

1 - Aplicação dos Stickers na imagem: o usuário deve clicar, na ordem correta na tela, no osso cujo nome 
aparece na tela secundária. Caso queira conferir a resposta, basta clicar em 'r'.
2 - Aplicação de filtros: o usuário pode escolher dentre os filtros da lista para aplicar na imagem do esqueleto. 
Há a opção de salvar a imagem desejada.
3 - Abertura da webcam: o usuário pode escolher abrir sua webcam com os filtros da lista, ou sem. 
Há a opção de salvar um frame da tela.

OPÇÃO 1:

	O objetivo é clicar no osso correto, seguindo a lista d os 5 nomes que aparece na tela secundária.
O usuário deve clicar na ordem correta, pois cada Sticker com o nome é colado apenas uma vez na tela. 
Um sexto clique na tela salva a imagem com os Stickers.

OPÇÃO 2:
	
	A imagem principal do esqueleto irá aparecer na tela e o usuário poderá escolher entre as opções 
de filtros disponíveis, que aparecerão na tela secundária. A seleção é feita com os números do teclado, 
de 0 a 9. 
	As opções de filtros de número 8 e 9 são opções de filtro de cor, onde estão configuradas para filtrar as imagens
referentes aos sistemas Nervoso e Circulatório. Cada filtro possui um código de cor em HSV referente às cores 
da imagem escolhida.
	O usuário pode salvar a imagem que preferir, clicando 's' no teclado. Cada nova foto salva tem o mesmo nome
da anterior, sobrepondo-a sobre o arquivo já existente na pasta. 

OPÇÃO 3:

	Abertura da webcam do usuário. Nessa sessão, será mostrada uma lista no display de 9 opções, acessadas 
pelo teclado, de filtros. O usuário deve escolher entre as opções de filtro ou sem filtro para abrir a webcam.
É possível salvar um frame da câmera, em cada uma das opções, pressionando 's' a qualquer momento. Para voltar 
à lista de seleção de filtros, basta clicar 'x'.
	As vezes, é necessário clicar mais de uma vez na tecla para fechar a webcam e voltar à lista de filtros.
	As imagens salvas não ficam com o filtro aplicado. 


OBS.: Ao salvar as imagens, elas aparecem na pasta junto com as outras imagens do programa, em:
"...opencv\build\samples\cpp"
LISTA DE IMAGENS DO PROGRAMA

filters.jpg
skel.jpg
stickers.jpg
falanges.png
femur.jpg
radio.png
tibia.png
umero.png
skel.png
skelresposta.png
nervsoso.png
veias.png

O trabalho foi feito utilizando uma das "samples" disponíveis ao baixar e instalar os arquivos do Opencv, portanto,
para abrir o arquivo .sln, deve-se escolher o de nome "OpenCV.sln" para inicializar o programa. Caso não esteja
selecionado, a solução tem nome "BasicLinearTransforms".

Os demais arquivos de imagens podem ser enconrtados em: "...opencv\build\samples\cpp".





