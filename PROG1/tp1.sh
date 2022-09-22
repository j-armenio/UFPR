echo -e 'Baixando o arquivo CSV:\n'
sleep 1
wget http://dadosabertos.c3sl.ufpr.br/curitiba/BaseAlvaras/2022-02-23_Alvaras-Base_de_Dados.CSV
echo ' '

echo 'Convertendo a codificação:'
sleep 1
iconv -f ISO_8859-1 -t UTF8 2022-02-23_Alvaras-Base_de_Dados.CSV > saida.CSV
echo -e 'Convertido para saida.CSV.\n'
sleep 1

echo 'Ruas em ordem alfabética:' 
sleep 1
cat saida.CSV | cut -d ';' -f7,10,15 | grep -iE "FARMA|DROGA" | grep -i BACACHERI | awk -F ";" '{print $2}' | sort
echo ' '
sleep 1

echo 'Quantidade de farmácias no Bairro Bacacheri:'
sleep 1
cat saida.CSV | cut -d ';' -f7,10,15 | grep -iE "FARMA|DROGA" | grep -i BACACHERI | awk -F ";" '{print $2}' | wc -l
