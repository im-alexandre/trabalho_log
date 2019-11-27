import csv
import os
import smtplib
from email.message import EmailMessage
import serial

entrada = serial.Serial('/dev/ttyACM0', 9600)
entrada.reset_input_buffer()


oficiais = os.path.join("/home/pi/scripts/oficiais.csv")
produtos = os.path.join("/home/pi/scripts/produtos.csv")
mensagem = os.path.join("/home/pi/scripts/mensagem.txt")
total = 0
# recebendo lista de emails
email = os.path.join("/home/pi/scripts/emails.csv")
dest = []
with open(email, "r") as arq:
    reader = csv.reader(arq)
    for row in reader:
        dest.append(row[2].lower())


# login no servidor smtp
s = smtplib.SMTP('smtp.gmail.com', 587)
s.starttls()
s.login('seu_email', 'sua_senha')

def checaProdutos(var):
    # ---- verifica se o código recebido é um produto
    with open(produtos, "r") as arq:
        reader = csv.reader(arq)
        for i in reader:
            if var in i:
                return i

def checaOficial(var):
    # --- verifica se o código recebido é uma identidade
    with open(oficiais, "r") as arq:
        reader = csv.reader(arq)
        for i in reader:
            if var in i:
                return i

def mandaEmail():
    # Abre um arquivo de texto simples. O arquivo deve estar em formato "string"
    # apenas caracteres ASCII
    msg = EmailMessage()
    with open(mensagem, "r") as fp:
        msg.set_content(fp.read())

    msg['Subject'] = 'Paiol do futuro - saída detectada'
    msg['From'] = "seu_email"
    msg['To'] = dest

    # Encaminha o e-mail através do servidor smtp do google
    try:
        s.sendmail('seu_email', dest, msg.as_string())
        s.quit()

    except:
        print('erro ao enviar')
    finally:
        with open(mensagem, "w") as arq:
            arq.write("")
            exit()


# ---- Programa principal ----

print("\n" + 25 * "*" + "PAIOL DO FUTURO" + 25 * "*" +"\n")

while True:

    com = entrada.readline()[1:-1]  # recebe os dados da porta serial (arduino + RFID)
    linha = com.decode("utf-8")     # traduz para string

    # se for produto, calcula o subtotal, cria a linha do texto e grava no arquivo
    if checaProdutos(linha):
        pd = checaProdutos(linha)
        line = pd[1] + "--------" + pd[2] + "\n"
        print(line)
        with open(mensagem, "a") as msg:
            msg.writelines(line)
            total += float(pd[2])

    # se for uma identidade autorizada, captura seus dados e grava no arquivo. Bem
    # como encaminha os dados por e-mail
    if checaOficial(linha):
        of = checaOficial(linha)
        line = "\n" + "\n" + "Retirada realizada pelo: " + of[1] + " " + of[2] + "\n"
        print(line)
        with open(mensagem, "a") as msg:
            msg.writelines(line)
            msg.writelines("Valor total: " + str(round(total,2)))
        mandaEmail()
