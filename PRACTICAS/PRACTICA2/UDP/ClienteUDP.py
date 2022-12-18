import socket
import os
import sys

def iniciamosSocket(datos):
    with socket.socket(socket.AF_INET,socket.SOCK_DGRAM) as aliasCliente:
        instrucciones(aliasCliente, datos)

def instrucciones(aliasCliente, dirIp):
    print("Ingrese Subir (Archivo a subir). Ingrese Salir para terminar")
    while True:
        linea = input("-> ")
        if linea == 'Salir':
            aliasCliente.sendto(linea.encode(), dirIp)
            break
        elif linea[:5] == 'Subir':
            if not os.path.exists(linea[6:]) is True:
                print("El archivo no existe!!")
            else:
                aliasCliente.sendto(linea.encode(), dirIp)
                enviamosArchivo(linea, aliasCliente, dirIp)
        else:
            print("La instruccion no existe")

def enviamosArchivo(linea, aliasCliente, dirIp):
    _ = aliasCliente.recv(10)
    try:
        with open(linea[6:], 'rb') as archivo:
            while True:
                buffer = archivo.read(200)
                if len(buffer) == 0:
                    buffer = b'fin'
                aliasCliente.sendto(buffer, dirIp)
                _ = aliasCliente.recv(2)
                if buffer == b'fin':
                    print("Archivo enviado!")
                    break
    except Exception as e:
        print(f"Error: {e}")

if __name__=='__main__':
    datos=("127.0.0.1", 8080)
    print(datos)
    iniciamosSocket(datos)