### build once
docker build -t pipex-dev .

### how to make it persistant: create a volume
docker volume create pipex-ws

### start container as a regular user
docker run --rm -it \
  --user 1000:1000 \
  --cap-drop=DAC_OVERRIDE \
  --cap-drop=DAC_READ_SEARCH \
  -v pipex-ws:/workspace \
  pipex-dev


# Multiple terminals against single container
For this to work we need to give container a name, so we can attach to it bash as user or root. 
Example for this, useful alias: 

alias pipex-dev-root='docker run -it --user root -v pipex-ws:/workspace pipex-dev'
alias pipex-dev-new='docker run --name pipex-dev-container -it --user 1000:1000 \
  --cap-drop=DAC_OVERRIDE --cap-drop=DAC_READ_SEARCH \
  -v pipex-ws:/workspace pipex-dev'
alias pipex-dev='docker exec -it --user 1000:1000 pipex-dev-container bash'

### How to use Vscode IDE to edit cloned code in container
Dev Containers extension->control+p->Dev Containers: Attach to running container