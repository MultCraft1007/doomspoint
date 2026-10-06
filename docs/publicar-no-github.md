# Publicar este fork no GitHub

Publique a pasta `crosspoint-reader-develop` como um repositório. Ela contém o código do CrossPoint modificado, a referência ao submódulo FreeInk e o firmware compilado em `firmware/CrossPoint-Doomsday-X4.bin`. A pasta vizinha `doomsdayclock` é o projeto web e pode ficar em outro repositório.

O firmware compilou para o Xteink X4, mas ainda não foi testado no aparelho. Não o identifique como versão estável até verificar as funções no dispositivo.

Crie um repositório **vazio** na sua conta do GitHub, sem inicializar README, licença ou `.gitignore`. No PowerShell, dentro desta pasta, execute:

```powershell
git config user.name "Seu nome no GitHub"
git config user.email "seu-email-de-commits@example.com"
git commit -m "Adiciona treino Doomsday offline ao CrossPoint para X4"
git remote add origin https://github.com/SEU-USUARIO/NOME-DO-REPOSITORIO.git
git push -u origin main
```

Os arquivos já estão preparados na área de stage do Git. Troque nome, e-mail, usuário e repositório pelos seus dados antes de executar os comandos. Não envie a pasta pela opção de upload de arquivos do site: a referência ao submódulo `freeink-sdk` depende do Git.

Para obter o código em outro computador depois da publicação:

```powershell
git clone --recurse-submodules https://github.com/SEU-USUARIO/NOME-DO-REPOSITORIO.git
```
