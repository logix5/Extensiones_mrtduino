# Extension_NextionTFT_mrtduino
Extensión para usar el TFT Nextion. Dispone de una librería para desde Arduino crear botones, barras de progreso, etc.En nuestro caso hemos implementado
el protocolo intercambiado por comandos, y hacer operaciones más simples. Requiere de formación en el manejo del TFT con su propio programa para crear interfaces de presentación
muy vistosos. EL display envía comandos que podemso recibir con las operaciones de recepción del puerto serie. Luego desd Arduino se puede interactuar con esas 
propiedades, o enviar comandos para pintar directamente.

El caso del Display usado es el NX3224T024_011R => propiedades https://www.itead.cc/wiki/Nextion_HMI_Solution

El programa para realizar interfaces en el Display es: http://nextion.itead.cc/download.html

Los comandos intercambiados son sacados de: https://www.itead.cc/wiki/Nextion_Instruction_Set