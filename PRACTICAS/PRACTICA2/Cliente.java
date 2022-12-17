import java.io.*;
import java.net.*;
import javax.swing.*;
public class Cliente{
    public static void main(String[] args) throws Exception{
        DataOutputStream objOOS = null;
        Socket objS = null;
        try{
            objS = new Socket("127.0.0.1", 8080);                       //Creamos nuestro Socket.
            objOOS = new DataOutputStream(objS.getOutputStream());      //Creamos nuestro objeto objOOS, que nos permite capturar todo lo que llegue a nuestro Socket
            JFileChooser objJFC = new JFileChooser();                   //Creamos este objeto para poder abrir una ventana para seleccionar nuestro archivo
            File archivoSeleccionado;
            while(true){
                int res = objJFC.showOpenDialog(null);                  //Capturamos la opcion que seleccione el usuario, si da clic en Aceptar, entonces subimos nuestro archivo
                if(res == JFileChooser.APPROVE_OPTION){
                    objOOS.writeInt(0);                                 //Enviamos el valor 0 para decirle al servidor que subiremos un archivo
                    archivoSeleccionado = objJFC.getSelectedFile();     //Obtenemos el archivo seleccionado.
                    String nombre = archivoSeleccionado.getName();      //Obtenemos el nombre del archivo
                    String ruta = archivoSeleccionado.getAbsolutePath();    //Obtenemos la ruta absoluta del archivo
                    long tam = archivoSeleccionado.length();                //Obtenemos el tamano total (en bytes) del archivo
                    DataInputStream objDIS = new DataInputStream(new FileInputStream(ruta));    //Creamos un objeto de tipo DataInputStream, que nos permitira abrir el archivo seleccionado en modo binario (con la clase FileInput)
                    objOOS.writeUTF(nombre);                            //Le enviamos al servidor el nombre del archivo
                    objOOS.flush();                                     //Limpiamos el canal/buffer
                    objOOS.writeLong(tam);                              //Enviamos el tamano total del archivo al servidor
                    objOOS.flush();                                     //Limpiamos el canal/buffer
                    long enviado = 0;
                    int a = 0, porcentaje = 0;
                    byte[] paquete = new byte[200];                     //Creamos nuestro paquete de bytes, con un tamano de 200 bytes
                    while(enviado < tam){                               //Realizamos un ciclo hasta que alcancemos el tamano total del archivo
                        a = objDIS.read(paquete);                       //Capturamos el total de bytes leidos del archivo y capturados en nuestro paquete
                        objOOS.write(paquete, 0, a);                    //Enviamos los bloques de bytes de nuestro archivo al servidor
                        objOOS.flush();                                 //Limpiamos el canal/buffer
                        enviado += a;                                   //Incrementamos nuestra variable que nos indica la cantidad de bytes enviados
                        porcentaje = (int)((enviado * 100) / tam);      //Operacion para sacar el porcentaje enviado
                        System.out.println("Enviado el "+porcentaje+"%");   //Imprimimos la operacion
                    }
                    System.out.println("Se ha subido el archivo");      //Mostramos mensaje
                    objDIS.close();                                     //Cerramos el archivo
                }else{objOOS.writeInt(1); break;}                       //Si el cliente ya no desea subir el arhcivo, entonces le enviamos al servidor el codigo 1 y rompemos el ciclo
            }
        }catch(Exception e){e.printStackTrace();}                       //Capturamos errores
        finally{
            if(objOOS != null){objOOS.close();}                         //Cerramos el objeto que nos permitia enviar nuestros datos
            if(objS != null){objS.close();}                             //Cerramos el Socket
        }
    }
}