#!/bin/bash

#set -x

NOBACKUP=/nobackup/bcc/jas21/
LINK=http://dadosabertos.c3sl.ufpr.br/curitiba/156/

function baixa_URLS {
	for DOWNLOADS in $URLS
	do
		wget -qnc "$LINK""$DOWNLOADS"
	done
}

#remove o "lixo" e converte para UTF-8
function conversao_utf8 {
	for DOWNLOADS in $URLS
	do
		sed 's/[^[:print:]]//g' "$DOWNLOADS" | iconv -f ISO-8859-1 -t UTF-8 -o ${DOWNLOADS}
	done
}

#remove as 2 primeiras linhas
function remocao_de_linhas {
	for DOWNLOADS in $URLS
	do
		sed -i 1,2d "${DOWNLOADS}"
	done
}

#cria um txt para cada bairro com seu Top 5
function top5_bairros {
	while read BAIRRO
	do
		cut -d ';' -f6,10 *.csv | grep -i "${BAIRRO}" | cut -d ';' -f1 | sort -f | uniq -c | sort -nr | head -n5 > assuntos_"${BAIRRO// /_}"_top5.txt
	done < todos_bairros.txt
}

#Cria um txt para cada data com seu Top 5
function top5_datas {
	while read DATAS
	do
		cut  -d ';' -f6 "${DATAS}"*.csv | sed '/^$/d' | sort -f | uniq -c | sort -nr | head -n5 > assuntos_"${DATAS}"_top5.txt
	done < datas.txt
}

#salva o diretorio inicial
pushd "$pwd" > /dev/null

#cria diretorio disque156 no nobackup
cd $NOBACKUP
mkdir -p disque156
cd disque156

#baixa o index.html
wget -qnc http://dadosabertos.c3sl.ufpr.br/curitiba/156/

#extrai as URLs das páginas para variável URLS
URLS=$(cat index.html | cut -d ' ' -f5 | grep csv | grep -v "Historico" | grep "2022" | cut -d '"' -f2)

baixa_URLS

conversao_utf8

remocao_de_linhas

#Lista todas strings da coluna ASSUNTO e a frequência de cada uma
cut -d ';' -f6 *.csv | sed '/^$/d' |  sort -f | uniq -c | sort -nr > assuntos_geral.txt

#Extrai todos os bairros em um txt
cut -d ';' -f10 *.csv | sed '/^$/d' | sort -fu > todos_bairros.txt

top5_bairros

rm -r todos_bairros.txt

#Extrai as datas
ls *.csv | cut -d "_" -f1 > datas.txt

top5_datas

rm -r datas.txt

mkdir -p arquivosCSV
mv *.csv arquivosCSV/

#retorna ao diretorio inicial
popd > /dev/null
