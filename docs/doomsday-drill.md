# Treino Doomsday offline

O treino aparece como **Treino Doomsday** na tela inicial (ou “Doomsday Drill” em inglês). Ele roda no leitor, sem navegador, Wi-Fi ou cartão SD durante a sessão.

O firmware para o Xteink X4 já foi compilado como [`CrossPoint-Doomsday-X4.bin`](../firmware/CrossPoint-Doomsday-X4.bin). A instalação por cartão SD e o funcionamento do treino, da rotação de wallpapers e do repouso por tempo foram confirmados pelo proprietário em um X4 com CrossPoint 1.6.0. Para instalar por USB:

1. Conecte o X4 ligado ao computador com um cabo USB-C que transfira dados.
2. Abra o [flasher oficial do CrossPoint](https://crosspointreader.com/#flash-tools) em Chrome ou Edge.
3. Escolha **X4** (não X4 Pro nem X4 Classic), depois **Custom .bin**.
4. Selecione `CrossPoint-Doomsday-X4.bin` e siga o processo de gravação até o fim.
5. Reinicie o leitor. **Treino Doomsday** estará na tela inicial.

Se o X4 **já tiver CrossPoint** e não for reconhecido por USB, use a atualização pelo cartão SD:

1. No X4, abra **Transferência** e escolha **Entrar em uma rede** ou **Criar hotspot**. Conecte o computador à mesma rede e abra, no navegador, o endereço IP mostrado pelo leitor.
2. Na página de arquivos, envie `CrossPoint-Doomsday-X4.bin` para a raiz do cartão. Saia de **Transferência** no X4.
3. Abra **Configurações > Sistema > Atualização de firmware via cartão SD**, escolha o `.bin` enviado e confirme. Aguarde a reinicialização sem desligar o aparelho.

Esse método depende do menu de atualização do CrossPoint; não é um procedimento para o firmware original da Xteink. Em aparelhos com gravação USB bloqueada, instalar um fork experimental pode dificultar a recuperação caso ele não inicialize.

O arquivo é uma imagem de aplicativo ESP32-C3 compilada no ambiente `gh_release`. O build também gera `firmware.factory.bin`; use o arquivo acima na opção **Custom .bin**. Se o navegador não detectar o aparelho, consulte as instruções sobre cabo, porta USB e aparelhos com gravação USB bloqueada no [README](../README.md#usb-locked-devices-xteink-unlocker).

No Xteink X4, use os botões: **Confirmar**, **Voltar**, **Esquerda** e **Direita** ficam na borda inferior; há também dois botões de página na lateral.

1. Leia a data e calcule mentalmente o dia da semana.
2. Pressione **Confirmar** para revelar a resposta e parar o cronômetro.
3. Pressione **Esquerda** ou o botão lateral de página anterior para marcar “Errei”. Pressione **Direita** ou o botão lateral de próxima página para marcar “Acertei”.
4. Pressione **Confirmar** para sortear outra data. **Voltar** encerra a sessão e retorna ao item na tela inicial.

São sorteadas datas do calendário gregoriano entre 1600 e 2099. A tela mostra também o dia âncora do ano, calculado a partir de 4 de abril. A média usa apenas as respostas marcadas como corretas, como no aplicativo web. Acertos, erros e média permanecem somente na memória da atividade e são zerados ao sair.

O cronômetro começa após a atualização da tela e para no comando de revelar; nenhuma atualização periódica da tela e-ink é feita durante a contagem. Para conferir o resultado no aparelho, teste datas conhecidas como 01/01/2000 (sábado), 29/02/2000 (terça-feira) e 05/10/2026 (segunda-feira).

O idioma do treino acompanha **Configurações > Idioma**. O modo noturno do aparelho inverte a tela do treino automaticamente.

Para alternar entre vários wallpapers ao dormir, salve imagens BMP na pasta `/sleep` do cartão SD, escolha **Configurações > Tela > Tela de repouso > Personalizada** e ative **Alternar papéis de parede**. A cada repouso, o firmware escolhe a próxima imagem na ordem em que o cartão lista os arquivos e volta à primeira depois da última. A escolha continua após reiniciar. Com a opção desligada, o comportamento original é preservado: `/sleep.bmp` tem prioridade; na falta dele, o firmware sorteia uma imagem da pasta. A rotação usa a pasta mesmo se `/sleep.bmp` existir.

Para mostrar o wallpaper após inatividade, ajuste **Tempo para repouso** para o número de minutos desejado e mantenha **Tela de repouso** em **Personalizada**. Nesse modo, o tempo limite mostra a imagem mesmo se **Retomada rápida após tempo limite** estiver ativada. Selecionar **Retomada rápida** como tela de repouso ainda mantém a página com o ícone de lua.
