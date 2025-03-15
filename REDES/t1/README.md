# Como compilar e testar no docker

Para o desenvolvimento, usamos um ambiente docker com dois containers, que possuem uma network entre eles.

## Requisitos

- `docker`
- `docker compose`

## Rodando

**Tem que rodar tudo com sudo!**

1. Execute o docker compose para subir os dois containers.

```shell
docker compose up --build -d
```

2. Em dois terminais, entre em cada container.

```shell
docker compose exec -it client /bin/bash
docker compose exec -it server /bin/bash
```

3. Dentro do container, compile os códigos do cliente e do servidor.

```shell
[cliente]: make
[cliente]: ./bin/client

[servidor]: make
[servidor]: ./bin/server
```

## Recompilando

Se foi feita alguma atualização no código, não é necessário subir os containers novamente, basta recompilar o código do serviço (`client` ou `servidor`) e rodar. As pastas ficam sincronizadas entre os containers e o diretório da máquina local.
