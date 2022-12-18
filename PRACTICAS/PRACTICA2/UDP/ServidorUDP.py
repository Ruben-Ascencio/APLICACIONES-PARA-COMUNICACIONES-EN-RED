import threading
import sys
import os
import socket

def iniciamosSocket(inforServer):
    with socket.socket(socket.AF_INET,socket.SOCK_DGRAM) as aliasServidor:
        aliasServidor.bind(inforServer)
        capturamosPaquetes(aliasServidor)

def capturamosPaquetes(aliasServidor):
    while True:
        paquete, data = aliasServidor.recvfrom(1024)
        if paquete.decode() == "exit":
            break
        elif paquete[:5].decode() == "Subir":
            creamosArchivo(paquete,aliasServidor,data)
    aliasServidor.close()

def creamosArchivo(paquete, aliasServidor, data):
    aliasServidor.sendto(b'1', data)
    os.makedirs("DocumentosClientes", exist_ok=True)
    os.chdir("DocumentosClientes")
    try:
        with open(paquete[7:],'wb') as Archivo:
            while True:
                buffer, data = aliasServidor.recvfrom(200)
                if buffer != b'fin':
                    Archivo.write(buffer)
                    aliasServidor.sendto(b'1', data)
                else:
                    print('Archivo descargado')
                    break
    except Exception as e:
        print(f"Error: {e}")
    os.chdir('..')
    aliasServidor.sendto(b'1', data)

if __name__ == '__main__':
    datos = ("127.0.0.1", 8080)
    print(datos)
    iniciamosSocket(datos)