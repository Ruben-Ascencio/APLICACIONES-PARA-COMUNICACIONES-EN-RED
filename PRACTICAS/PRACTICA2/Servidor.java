import java.io.*;
import java.net.*;
import java.time.*;
import java.time.LocalDateTime;
public class Servidor{
    public static void main(String[] args) throws Exception{
        Socket objS = null;                                         //Iniciamos nuestro Socket en null
        ServerSocket objSS = new ServerSocket(8080);                //Objeto cuya funcion es equivalente a la funcion accept en C
        while(true){                                                //Ciclo infinito
            try{
                objS = objSS.accept();                              //Aceptamos la conexion entrante
                (new Ejecucion(objS)).start();                      //Arrancamos nuestro metodo Ejecucion con un hilo
            }catch(Exception e){e.printStackTrace();}               //Capturamos errores
        }
    }
    static class Ejecucion extends Thread{
        private Socket objS = null;
        private DataInputStream objOIS = null;;
        public Ejecucion(Socket objS){this.objS = objS;}            //Constructor de la clase
        @Override
        public void run(){
            try{
                objOIS = new DataInputStream(objS.getInputStream());//Creamos un objeto de tipo DataInputStream para capturar los datos de entrada de nuestro socket
                LocalDateTime objLDT = LocalDateTime.now();         //Creamos un objeto de tipo LocalDateTime
                /* Bloque donde creamos las carpetas para cada usuario, el nombre de nuestras carpetas concatena la hora, minuto, segundo y numero del dia*/
                String direccion = "DOCUMENTOS/" + objLDT.getHour() + "_" + objLDT.getMinute() + "_" + objLDT.getSecond() + "_" + objLDT.getDayOfYear();
                File directorio = new File(direccion);
                if(!directorio.exists()){
                    if(directorio.mkdirs()){System.out.println("Carpeta creada correctamente");}
                    else{System.out.println("No se creo la carpeta"); System.exit(0);}
                }else{System.out.println("La carpeta ya existe");System.exit(0);}
                while(true){
                    int llave = objOIS.readInt();                   //Capturamos la llave enviada del cliente, si es 1, rompemos el ciclo. De lo contrario, el cliente subira un archivo
                    if(llave == 0){
                        String nombre = objOIS.readUTF();           //Obtenemos el nombre del archivo
                        long tam = objOIS.readLong();               //Obtenemos el tamano del archivo
                        DataOutputStream objDOS = new DataOutputStream(new FileOutputStream(direccion + "/" + nombre));     //Creamos el archivo y lo escribiremos en su respectiva carpeta
                        long recibido = 0;
                        int a = 0;
                        byte[] paquete =  new byte[200];            //Creamos nuestro paquete de bytes que recibira todos los bytes de los archivos seleccionados
                        while(recibido < tam){                      //Realizamos un ciclo hasta que alcancemos el tamano total del archivo
                            a = objOIS.read(paquete);               //Capturamos el total de bytes recibidos del archivo y capturados en nuestro paquete
                            objDOS.write(paquete, 0, a);            //Escribimmos nuestro archivo
                            objDOS.flush();                         //Limpiamos el buffer
                            recibido += a;                          //Incrementamos nuestra variable que nos indica la cantidad de bytes recibidos
                        }
                        System.out.println(nombre);                 //Imprimimos el nombre del archivo
                        objDOS.close();                             //Cerramos nuestro archivo
                    }else{break;}                                   //Rompemos la ejecucion del hilo si el cliente ya no desea subir un archivo
                }
            }catch(Exception e){e.printStackTrace();}               //Capturamos errores
            finally{
                try{
                    if(objOIS != null){objOIS.close();}             //Cerramos el objeto que nos permite recibir lo que mande el cliente
                    if(objS != null){objS.close();}                 //Cerramos el canal
                }catch(Exception ex){ex.printStackTrace();}         //Capturamo errores
            }
        }
    }
}