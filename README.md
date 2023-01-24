# ssh
Servidor y cliente SSH para sistemas Linux. Escrito con lenguaje C.
Proyecto final del curso "Arquitectura Cliente Servidor" de la Facultad de Ingeniería, UNAM.

## Funcionamiento

Se ejecuta el programa `servidorSSH` indicando el puerto donde se abrirá el servidor. Posteriormente se inicia el cliente indicando la IP y el puerto a buscar.

Cuando la conexión es exitosa, se muestra en pantalla el símbolo `$`, donde ya podrás mandar comandos sencillos.

Es posible enviar diferentes comandos y sus parámetros. El cliente recibirá la salida ejecutada en el servidor. En caso de error, se mostrará el error en la terminal del cliente.

**IMPORTANTE**

El cliente no permite mandar líneas vacías, es decir, no puedes dar "Enter" muchas veces.

Hay ocasiones donde la salida es muy grande y se puede trabar a la hora de enviar nuevos comandos, teniendo una respuesta retrasada.

Lo anterior puede ocurrir igual con algunos errores que manden dos mensajes.
