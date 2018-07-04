import RPi.GPIO as GPIO
import sys  # module to make call system
import MySQLdb as mdb  # connector for MySQL dataBase
import datetime  # date management libraries
import serial
import time
from xbee import XBee, ZigBee, digimesh

conn = False

data_available = 0
output = 1


def timeout(data):
    print data


def setstatus(endp_id, status):
    try:
        # Connect to Data Base MySQL#######Connect to Data Base MySQL
        con = mdb.connect('localhost', 'root', 'root','ardusense')
        with con:
            cur = con.cursor()
            # update ardusense.endpoints set endp_address='0013A200:408BC81E' where endp_id='endp_1'
            print("update ardusense.endpoints set state =" + str(status) + " where endp_id='" + endp_id + "'")
            cur.execute("update ardusense.endpoints set state =" + str(status) + " where endp_id='" + endp_id + "'")

    except con.Error as err:
        if output == 1:
            print("Something went wrong, working with  database: {}".format(err))
            serial_port.close()
            sys.exit(0)


def getendpoints():
    endp_params = {}
    try:
        con = mdb.connect('localhost', 'root', 'root', 'ardusense')
        with con:
            cur = con.cursor()
            # update ardusense.endpoints set endp_address='0013A200:408BC81E' where endp_id='endp_1'
            #print("SELECT endp_id,endp_address FROM ardusense.endpoints;")
            print "Recuperando datos de nodos de la red"
            cur.execute("SELECT endp_id,endp_address FROM ardusense.endpoints order by id asc;")
            endp_params = cur.fetchall()
            #print endp_params
            return endp_params
    except con.Error as err:


        if (output == 1):
            print("Something went wrong, working with  database: {}".format(err))
        print endp_params
        return endp_params
        serial_port.close()
        sys.exit(0)


def getrequesttime():

    endp_params = {}

    try:
        #onnect to Data Base MySQL#######Connect to Data Base MySQL
        con = mdb.connect('localhost', 'root', 'root', 'ardusense')
        with con:
            cur = con.cursor()
            # update ardusense.endpoints set endp_address='0013A200:408BC81E' where endp_id='endp_1'
            print("SELECT request_time FROM ardusense.managers;")
            cur.execute("SELECT request_time FROM ardusense.managers;")
            endp_params = cur.fetchone()
            print "Hora de pedida de datos: ",
            print endp_params[0]
            return endp_params[0].second
    except con.Error as err:
        return endp_params
        if (output == 1):
            print("Something went wrong, working with  database: {}".format(err))
        serial_port.close()
        sys.exit(0)


def saveinDB(params):
    global output
    try:
        con = mdb.connect('localhost', 'root', 'root','ardusense')
        with con:

            print params['time']
            aux_t = datetime.datetime.utcfromtimestamp(float(params['time'])).strftime('%Y-%m-%d %H:%M:%S')

            print aux_t

            cur = con.cursor()
            if (output == 1):
                """
                print(
                "INSERT INTO ardusense.data (stime,loctime,endp_id,value,units,offset,datatype,samplfreq,channel_id) VALUES('" + str(
                    aux_t) + "','" + str(datetime.datetime.now()) + "','" + params['source'] + "'," + str(
                    params['Temp1']) + ",'C',0,'temperatura','0.001','temp_1')")
                """
                cur.execute(
                    "INSERT INTO ardusense.data (stime,loctime,endp_id,value,units,offset,datatype,samplfreq,channel_id) VALUES('" + str(
                        aux_t) + "','" + str(datetime.datetime.now()) + "','" + params['source'] + "'," + str(
                        params['Temp1']) + ",'C',0,'temperatura','0.001','temp_1')")
                """
                print(
                "INSERT INTO ardusense.data (stime,loctime,endp_id,value,units,offset,datatype,samplfreq,channel_id) VALUES('" + str(
                    aux_t) + "','" + str(datetime.datetime.now()) + "','" + params['source'] + "'," + str(
                    params['Temp2']) + ",'C',0,'temperatura','0.001','temp_2')")
                """
                cur.execute(
                    "INSERT INTO ardusense.data (stime,loctime,endp_id,value,units,offset,datatype,samplfreq,channel_id) VALUES('" + str(
                        aux_t) + "','" + str(datetime.datetime.now()) + "','" + params['source'] + "'," + str(
                        params['Temp2']) + ",'C',0,'temperatura','0.001','temp_2')")
                """
                print(
                "INSERT INTO ardusense.data (stime,loctime,endp_id,value,units,offset,datatype,samplfreq,channel_id) VALUES('" + str(
                    aux_t) + "','" + str(datetime.datetime.now()) + "','" + params['source'] + "'," + str(
                    params['Hum1']) + ",'%',0,'humedad','0.001','hum_1')")
                """
                cur.execute(
                    "INSERT INTO ardusense.data (stime,loctime,endp_id,value,units,offset,datatype,samplfreq,channel_id) VALUES('" + str(
                        aux_t) + "','" + str(datetime.datetime.now()) + "','" + params['source'] + "'," + str(
                        params['Hum1']) + ",'%',0,'humedad','0.001','hum_1')")
                """"
                print(
                "INSERT INTO ardusense.data (stime,loctime,endp_id,value,units,offset,datatype,samplfreq,channel_id) VALUES('" + str(
                    aux_t) + "','" + str(datetime.datetime.now()) + "','" + params['source'] + "'," + str(
                    params['Hum2']) + ",'%',0,'humedad','0.001','hum_2')")
                """
                cur.execute(
                    "INSERT INTO ardusense.data (stime,loctime,endp_id,value,units,offset,datatype,samplfreq,channel_id) VALUES('" + str(
                        aux_t) + "','" + str(datetime.datetime.now()) + "','" + params['source'] + "'," + str(
                        params['Hum2']) + ",'%',0,'humedad','0.001','hum_2')")
                cur.execute(
                    "INSERT INTO ardusense.data (stime,loctime,endp_id,value,units,offset,datatype,samplfreq,channel_id) VALUES('" + str(
                        aux_t) + "','" + str(datetime.datetime.now()) + "','" + params['source'] + "'," + str(
                        float(params['bat'])/1000) + ",'V',0,'voltaje','0.001','bat')")
                cur.execute(
                    "INSERT INTO ardusense.data (stime,loctime,endp_id,value,units,offset,datatype,samplfreq,channel_id) VALUES('" + str(
                        aux_t) + "','" + str(datetime.datetime.now()) + "','" + params['source'] + "'," + str(
                        params['bat_status']) + ",'Est',0,'estado','0.001','bat_status')")

                print "Datos guardados correctamente"

    except con.Error as err:
        if (output == 1):
            print("Something went wrong, working with  database: {}".format(err))
            serial_port.close()
            sys.exit(0)


def handledata(frame, nframe, type, source_endp):
    global data_available
    global output
    # global xbee
    # data_available=nframe
    params = {}
    try:
        """
        if type == 0:
            print "cuantos debo esperar" + str((frame['rf_data']))
            data_available = (frame['rf_data'])
        """
        if type == 1:
            print("tenemos que guardar")
            print frame['rf_data'].split("?")[0]
            print frame['rf_data'].split("?")[0].split("$")[2]
            print("nodo:")
            print frame['rf_data'].split("?")[0].split("$")[1]
            params['source'] = frame['rf_data'].split("?")[0].split("$")[1]
            print ("hora:")
            print frame['rf_data'].split("?")[0].split("$")[3]
            params['time'] = frame['rf_data'].split("?")[0].split("$")[3]
            print ("temperatura 1:")
            print frame['rf_data'].split("?")[0].split("$")[2].split(";")[0]
            params['Temp1'] = frame['rf_data'].split("?")[0].split("$")[2].split(";")[0]
            print("temperatura 2:")
            print frame['rf_data'].split("?")[0].split("$")[2].split(";")[1]
            params['Temp2'] = frame['rf_data'].split("?")[0].split("$")[2].split(";")[1]
            print("humedad 1:")
            print frame['rf_data'].split("?")[0].split("$")[2].split(";")[2]
            params['Hum1'] = frame['rf_data'].split("?")[0].split("$")[2].split(";")[2]
            print ("humedad 2:")
            print frame['rf_data'].split("?")[0].split("$")[2].split(";")[3]
            params['Hum2'] = frame['rf_data'].split("?")[0].split("$")[2].split(";")[3]
            print ("nivel bateria:")
            print frame['rf_data'].split("?")[0].split("$")[2].split(";")[4]
            params['bat'] = frame['rf_data'].split("?")[0].split("$")[2].split(";")[4]
            print ("Estado de la carga:")
            print frame['rf_data'].split("?")[0].split("$")[2].split(";")[5]
            params['bat_status'] = frame['rf_data'].split("?")[0].split("$")[2].split(";")[5]
            # params['Hum2']=frame['rf_data'].split("?")[0].split("$")[1]
            # params['Temp1']=frame['rf_data'].split("?")[0].split("$")[1]

            print params
            saveinDB(params)
            print "Ye he guardado los datos"
            # xbee.send("tx",id="\x10",frame_id="\x01",dest_addr_long=address.decode("hex"),dest_addr="\xFF\xFE",broadcast_radius="\x00",options="\x00",data="x30")

        else:
            if output == 1:
                print "No manejamos este tipo de trama"
        return data_available

        # """for i in range(6):
        # GPIO.output(13, GPIO.HIGH)
        # GPIO.output(26, GPIO.HIGH)
        # time.sleep(0.1)
        # GPIO.output(13, GPIO.LOW)
        # GPIO.output(26, GPIO.LOW)
        # time.sleep(0.1)"""
    except Exception as e:
        data_available = 0
        if output == 1:
            print "Fallo al manejar los datos"
            print e
        serveforever()
        return data_available


def rqlendata(dest_addr, source_endp, xbee):
    global data_available
    # global xbee
    global serial_port
    discovered = False

    print "Peticion numero de lineas"

    xbee.send("tx", id="\x10", frame_id="\x01", dest_addr_long=dest_addr.decode("hex"), dest_addr="\xFF\xFE",
              broadcast_radius="\x00", options="\x00", data="\x10")

    #i = 0
    while True:
        # i =i+1
        #frame = 'vacio'
        try:
            xbee.halt()
            frame = xbee.wait_read_frame(timeout = 2)
            if frame['id'] == 'rx':
                # print frame
                if frame['rf_data'].split("$")[1] == "av_data":
                    # print frame
                    data_available = frame['rf_data'].split("$")[2].split('?')[0]
                    if int(data_available) == 999:
                        print "trama  corrupta 999"
                    elif int(data_available) == 1998:
                        print "data no del api"
                    else:
                        # print "numero de lineas disponibles"+data_available
                        print "numero de ficheros disponibles " + data_available
                        discovered = True
                        break
                else:
                    print "reintento"
                    discovered = False
            else:
                print "frame no handle 2"
                discovered = False
                # xbee.send("tx",id="\x10",frame_id="\x01",dest_addr_long=dest_addr.decode("hex"),dest_addr="\xFF\xFE",broadcast_radius="\x00",options="\x00",data="\x10")
                # print frame
                # return
        except:
            print "error al recibir frame"
            discovered = False
            break
    xbee.halt()
    return discovered


def rq_data(dest_addr, source_endp, xbee):
    global data_available
    # global xbee
    # print ("mando 0x20")
    #print (data_available)
    print ("Peticion de ficheros")
    xbee.send("tx", id="\x10", frame_id="\x01", dest_addr_long=dest_addr.decode("hex"), dest_addr="\xFF\xFE",
              broadcast_radius="\x00", options="\x00", data="\x20")
    lin = 0
    i = 0
    while (i < int(data_available)):
        try:
            xbee.halt()
            frame = xbee.wait_read_frame(timeout = 2)
            print frame
            if frame['id'] == 'rx':
                #print frame
                if frame['rf_data'].split("$")[1] == "lineas_enviadas":
                    i = i + 1
                    print "fichero numero " + str(i)
                    print "enviado"
                    lin = 0
                elif frame['rf_data'].split("$")[1] == source_endp:
                    print "trama con datos"
                    #print frame
                    handledata(frame, data_available, 1, source_endp)
                    lin = lin + 1
                    print "linea " + str(lin)
                    print "fichero " + str(i + 1)
                elif frame['rf_data'].split("$")[1] == "error envio de datos":
                    print "error de conexion"
                else:
                    print "dato erroneo"

                if i == int(data_available):
                    print "ya no hay mas datos disponibles del " + source_endp
                    i = 0
                    break
                else:
                    pass
                    #print "no es tiempo de recibir numero de lineas"
                    #print frame
            else:
                print "no tx frame:"
                #print frame
        except:
            print "error al recibir fichero"
            break
    xbee.halt()

def end_tx(dest_addr, source_endp, xbee):
    print ("Peticion de confirmacion")
    xbee.send("tx", id="\x10", frame_id="\x01", dest_addr_long=dest_addr.decode("hex"), dest_addr="\xFF\xFE",
              broadcast_radius="\x00", options="\x00", data="\x30")
    while True:
        try:
            xbee.halt()
            frame = xbee.wait_read_frame(timeout = 2)
            if frame['id'] == 'rx':
                if frame['rf_data'].split("$")[1] == "ack_endtx":
                    #print frame
                    print "termino la transmision"
                    break
                else:
                    print"trama no correcta"
                    #print frame
            else:
                print "trama no Rx:"
        except:
            print "error confirmacion"
            break
            #print frame
    xbee.halt()

def serveforever(xbee):
    global data_available

    # global serial_port
    try:
        op = int(input("Introduce la opcion a ejecutar (h para ver la ayuda)"))
        print op
        if op == 1:
            xbee.send("tx", id="\x10", frame_id="\x01", dest_addr_long="\x00\x13\xA2\x00\x40\x8B\xC8\x1E",
                      dest_addr="\xFF\xFE", broadcast_radius="\x00", options="\x00", data="\x10")
            print "envio una peticion 10"
            while True:
                frame = xbee.wait_read_frame()
                if frame['id'] == 'rx':
                    if frame['rf_data'].split("$")[1] == "av_data":
                        print "trama av_data"
                        print frame
                        data_available = frame['rf_data'].split("$")[2].split('?')[0]
                        if int(data_available) == 999:
                            # print frame
                            print "trama  corrupta"
                            print frame
                        elif int(data_available) == 1998:
                            # print frame
                            print "data no del api"
                            print frame
                        else:
                            print "numero de lineas disponibles" + data_available
                            # print frame
                            break
                    elif frame['rf_data'].split("$")[1] == "endp_1":
                        print "no es momento de guadar datos"

                else:
                    print "trama no RX:"
                    print frame
        elif op == 2:
            print ("mando 0x20")
            print (data_available)
            xbee.send("tx", id="\x10", frame_id="\x01", dest_addr_long="\x00\x13\xA2\x00\x40\x8B\xC8\x1E",
                      dest_addr="\xFF\xFE", broadcast_radius="\x00", options="\x00", data="\x20")
            i = 0
            while i < int(data_available):
                frame = xbee.wait_read_frame()
                if frame['id'] == 'rx':
                    print frame['rf_data'].split("$")[1]
                    if frame['rf_data'].split("$")[1] == "endp_1":
                        i = i + 1
                        print "trama con datos"
                        print frame
                        handledata(frame, data_available, 1, "endp_1")
                        # saveinDB(params)
                        # handledata(frame,data_available,1,frame['source_addr_long'])
                        print "pedidas" + str(i)
                        print "available" + str(data_available)
                        if i >= int(data_available):
                            # xbee.halt()
                            print "ya no hay mas datos disponibles del endp_1"
                            break
                    else:
                        print "trama corrupta"
                        print frame
                else:
                    print "trama no rx"
                    print frame

        elif op == 3:
            while True:
                frame = xbee.wait_read_frame()
                print frame
                if frame['id'] == 'rx':
                    print "estoy vivo"
                    print frame['rf_data']
                break
        elif op == 4:
            print ("mando 0x70")
            print (data_available)
            xbee.send("tx", id="\x10", frame_id="\x01", dest_addr_long="\x00\x00\x00\x00\x00\x00\xff\xff",
                      dest_addr="\xFF\xFE", broadcast_radius="\x00", options="\x00", data="\x70")
        elif op == 5:
            print str(int(time.time()))
            #request_done = False
            completo = False
            while 1:
                t = datetime.datetime.now()
                #getrequesttime()
                #time.sleep(2)
                while True:
                    # if not (t.second>=10 and t.second<30):
                    t = datetime.datetime.now()
                    if (t.minute != 00 ):
                        #request_done = False
                        completo = False
                        #t = datetime.datetime.now()
                        #print (time.time())
                        print str(t.hour) + ":" + str(t.minute) + ":" + str(t.second)
                        time.sleep(5)

                    if ((t.minute == 00) and (t.hour == 12 ) and (completo == True)):
                        completo = True
                        print "esperando un nuevo ciclo"
                        time.sleep(5)
                        print "esperando un nuevo ciclo"

                    if ((t.minute == 00) and (t.hour == 12) and (completo == False)):
                        print ("secuencia de pedida de datos")
                        destinators = getendpoints()
                        # print destinators
                        time.sleep (90)#esperamos 1:30 minutos y asi nos aseguramos que los nodos se han despertado
                        for destinator in destinators:
                            print "destino de request" + destinator[0]
                            # print destinator
                            address = destinator[1].split(':')[0]
                            address += destinator[1].split(':')[1]
                            # serial_port = serial.Serial('/dev/ttyUSB0', 9600)
                            # xbee = ZigBee(serial_port, escaped=True)
                            discovered = False
                            discovered = rqlendata(address, destinator[0], xbee)
                            time.sleep(0.5)
                            if discovered:
                                # xbee = ZigBee(serial_port, escaped=True)
                                rq_data(address, destinator[0], xbee)
                                end_tx(address, destinator[0], xbee)
                                #request_done = False
                                completo = True
                                print "esperamos a un nuevo ciclo"
                            else:
                                print "datos ya pedidos esperamo al siguiente ciclo"
                                t = datetime.datetime.now()
                                print str(t.hour) + ":" + str(t.minute) + ":" + str(t.second)
                                time.sleep(0.5)
                                #request_done = False
                                completo = True
                            print "he terminado todos los datos"
                        print "he salido del bucle for"
                        #guardamos los datos de intensidad de la red
                        RSSI_Coordinador(xbee)
                        RSSI_resto_nodos(xbee)
                        print ("Sincronizamos la hora")
                        xbee.send("tx", id="\x10", frame_id="\x01", dest_addr_long="\x00\x00\x00\x00\x00\x00\xff\xff",dest_addr="\xFF\xFE", broadcast_radius="\x00", options="\x00",data="\x40" + str(time.time()))
                        print (time.time())

        elif op == 6:
            print ("mando 0x30")
            xbee.send("tx", id="\x10", frame_id="\x01", dest_addr_long="\x00\x13\xA2\x00\x40\x8B\xC8\x1E",
                      dest_addr="\xFF\xFE", broadcast_radius="\x00", options="\x00", data="\x30")
            while True:
                frame = xbee.wait_read_frame()
                if frame['id'] == 'rx':
                    if frame['rf_data'].split("$")[1] == "ack_endtx":
                        print "trama av_data"
                        print frame
                        break
                    else:
                        print"trama no correcta"
                        print frame
                else:
                    print "trama no Rx:"
                    print frame
        elif op == 7:
            print ("mando 0x40")
            xbee.send("tx", id="\x10", frame_id="\x01", dest_addr_long="\x00\x00\x00\x00\x00\x00\xff\xff",
                      dest_addr="\xFF\xFE", broadcast_radius="\x00", options="\x00", data="\x40" + str(time.time()))
            print (time.time())
        elif op == 8:
            print ("mando 0x50")
            op_t = int(input("Introduce el intervalo de toma de tiempo"))
            if (op_t >= 2) and (op_t <= 10):
                xbee.send("tx", id="\x10", frame_id="\x01", dest_addr_long="\x00\x13\xA2\x00\x40\x8B\xC8\x1E",
                          dest_addr="\xFF\xFE", broadcast_radius="\x00", options="\x00", data="\x50" + str(op_t))
            else:
                print("valor no valido")
        elif op == 9:
            destinators = getendpoints()
            i=0
            for destinator in destinators:
                print  "Para elegir el nodo " + destinator[0] + ": introduzca un " + str (i) #+ destinator[1]
                i= i+1
            #address = destinator[1].split(':')[0]
            #address += destinator[1].split(':')[1]
            #print (address)

            while True:
                op_nodo = int(input("introduzca opcion seleccionada: "))
                if op_nodo < i:
                    print ("Nodo elegido: ") + destinators[op_nodo][0]
                    address = destinators[op_nodo][1].split(':')[0]
                    address += destinators[op_nodo][1].split(':')[1]
                    #print address
                    break
                else:
                    print ("nodo no valido ")
                    #print ("Seleccione un nodo valido")

            print ("mando 0x60")
            op_file = int(input("introduzca numero de fichero a buscar: "))
            print (str(op_file))
            xbee.send("tx", id="\x10", frame_id="\x01", dest_addr_long=address.decode("hex"), dest_addr="\xFF\xFE",
                      broadcast_radius="\x00", options="\x00", data="\x60" + str(op_file) + ":")
            cont = 0

            while True:
                try:
                    xbee.halt()
                    frame = xbee.wait_read_frame(timeout = 2)
                    if frame['id'] == 'rx':
                        try:
                            if frame['rf_data'].split("$")[1] == str(destinators[op_nodo][0]):
                                handledata(frame, 0, 1, str(destinators[op_nodo][0]))
                                cont = cont + 1
                            elif frame['rf_data'].split("$")[1] == "lineas_enviadas":
                                print "Datos esperados: "
				print cont
				print "Datos recibidos: "
                                print frame['rf_data'].split("?")[0].split("$")[2]
                            if cont == int(frame['rf_data'].split("?")[0].split("$")[2]):
                                print "Datos recibidos correctamente"
                                break
                            else:
                                print ("no se recibieron todos los datos")
                        except:
                            #print cont
                            #print frame['rf_data'][0]
			    pass
                    else:
                        #print "trama corrupta"
			pass
                except:
                    print "El fichero que busca no existe o se produjo un error al recibir frame"
		    print "Intente nuevamente."
                    break
            xbee.halt()

        else:
            print("opcion invalida pulse h para ayuda")
            print ("Pulse 1 para recolectar lineas disponibles")
            print ("Pulse 2 para recolectar datos")
            print ("Pulse 4 para mantener despierta mas tiempo")
            print ("Pulse 6 para finalizar transmision")
            print ("Pulse 7 sincronizacion")
            print ("Pulse 8 modificar frecuencia de mustreo")
            print ("Pulse 9 peticion de fichero especifico")
            print ("pulse 5 para servicio automatico de recolecta de datos")

        serveforever(xbee)
    except Exception as e:

        print e

def RSSI_Coordinador(xbee):
    print 'pido dato RSSI de nodo coordinador'
    xbee.halt()
    # xbee.send("remote_at", id="\x17", frame_id="\x01", dest_addr_long="\x00\x13\xA2\x00\x40\xB1\x35\x5D", dest_addr="\xFF\xFE", options="\x00", command="\x46\x4E")
    xbee.send("at", frame_id="\x01", command="\x46\x4E")
    while True:
        try:
            frame = xbee.wait_read_frame(timeout=10)
            if frame['id'] == 'at_response' and frame['status'] == '\x00':
                print str(frame)
                try:
                    datosframe = frame['parameter']
                    datonodo = datosframe[10:17]
                    print datonodo
                    longi = len(datosframe)
                    datodb = datosframe[longi - 1]
                    print 'signal db: '
                    print ord(datodb)
                    nodo_emisor = str('nodo_co')
                    print nodo_emisor
                    nodo_receptor = str(datonodo)
                    print nodo_receptor
                    t = datetime.datetime.now()
                    fecha = str(t.day) + "/" + str(t.month) + "/" + str(t.year)
                    print fecha
                    insert_RSSI(str(fecha), nodo_emisor, nodo_receptor, datodb)
                    # consulta(longi, datodb, nodo_emisor, nodo_receptor)
                except:
                    break
            else:
                print 'no hay frame'
        except:
            print 'termino'
            break
    print 'paso al siguiente'
    # RSSI_resto_nodos(xbee)

    """
    print ("pido dato RSSI de nodo 10")
    xbee.halt()
    xbee.send("remote_at", id="\x17", frame_id="\x01", dest_addr_long="\x00\x13\xA2\x00\x40\xB1\x34\xDF",
      dest_addr="\xFF\xFE", options="\x00", command="\x46\x4E")
    while True:
        try:
            frame = xbee.wait_read_frame(timeout = 15)
            if frame['id'] == 'remote_at_response' and frame['status'] == '\x00':
                print str(frame)
                try:
                    datosframe = frame['parameter']
                    datonodo = datosframe[10:17]
                    print datonodo
                    longi = len(datosframe)
                    datodb = datosframe[longi - 1]
                    print 'signal db: '
                    print ord(datodb)
                    nodo_emisor = str('nodo_10')
                    print nodo_emisor
                    nodo_receptor = str(datonodo)
                    print nodo_receptor
                    t = datetime.datetime.now()
                    fecha = str(t.day) + "/" + str(t.month) + "/" + str(t.year)
                    print fecha
                    consulta(str(fecha), nodo_emisor, nodo_receptor, datodb)
                except:
                    break
            else:
                print 'no hay frame'
        except:
            print 'termino'
            break
    print 'paso al siguiente'
    print ("pido dato RSSI de nodo 2")
    xbee.halt()
    xbee.send("remote_at", id="\x17", frame_id="\x01", dest_addr_long="\x00\x13\xA2\x00\x40\xAD\x63\xA7",
              dest_addr="\xFF\xFE", options="\x00", command="\x46\x4E")
    #xbee.send("remote_at", id="\x17", frame_id="\x01", dest_addr_long="\x00\x13\xA2\x00\x40\xB1\x35\x5D", dest_addr="\xFF\xFE", options="\x00", command="\x46\x4E")
    while True:
        try:
            frame = xbee.wait_read_frame(timeout = 15)
            if frame['id'] == 'remote_at_response' and frame['status'] == '\x00':
                print str(frame)
                try:
                    datosframe = frame['parameter']
                    datonodo = datosframe[10:17]
                    print datonodo
                    longi = len(datosframe)
                    datodb = datosframe[longi - 1]
                    print 'signal db: '
                    print ord(datodb)
                    nodo_emisor = str('nodo_2')
                    print nodo_emisor
                    nodo_receptor = str(datonodo)
                    print nodo_receptor
                    t = datetime.datetime.now()
                    fecha = str(t.day) + "/" + str(t.month) + "/" + str(t.year)
                    print fecha
                    consulta(str(fecha), nodo_emisor, nodo_receptor, datodb)
                except:
                    break
            else:
                print 'no hay frame'
        except:
            print 'termino'
            break
    print 'paso al siguiente ciclo'
    """
    time.sleep(5)
    # RSSI_GLOBAL(xbee)


def consulta_nodos(xbee):
    try:
        con = mdb.connect('localhost', 'root', 'root', 'ardusense')
        with con:
            cur = con.cursor()
            # update ardusense.endpoints set endp_address='0013A200:408BC81E' where endp_id='endp_1'
            # print("SELECT endp_id,endp_address FROM ardusense.endpoints;")
            print "Recuperando datos de nodos de la red"
            # cur.execute("SELECT nodo,endp_address FROM ardusense.endpoints where nodo='nodo_10' or nodo='nodo_02' order by id asc;")
            cur.execute(
                "SELECT nodo,endp_address FROM ardusense.endpoints;")
            end_params = cur.fetchall()
            return end_params
    except con.Error as err:
        if (output == 1):
            print("Something went wrong, working with  database: {}".format(err))
        return end_params
        serial_port.close()
        sys.exit(0)
    # time.sleep(10)  # esperamos 1:30 minutos y asi nos aseguramos que los nodos se han despertado


def RSSI_resto_nodos(xbee):
    destinators = consulta_nodos(xbee)
    for destinator in destinators:
        address = destinator[1].split(':')[0]
        address += destinator[1].split(':')[1]
        time.sleep(0.5)
        # address="0013A20040B134DF"
        # address = "0013A20040AD63A7"
        xbee.halt()
        xbee.send("remote_at", id="\x17", frame_id="\x01", dest_addr_long=address.decode('hex'),
                  dest_addr="\xFF\xFE", options="\x00", command="\x46\x4E")
        print ("pido dato RSSI desde: " + destinator[0])
        print ("direccion: " + address)
        while True:
            try:
                frame = xbee.wait_read_frame(timeout=10)
                if frame['id'] == 'remote_at_response' and frame['status'] == '\x00':
                    print str(frame)
                    try:
                        datosframe = frame['parameter']
                        datonodo = datosframe[10:17]
                        print datonodo
                        longi = len(datosframe)
                        datodb = datosframe[longi - 1]
                        print 'signal db: '
                        print ord(datodb)
                        nodo_emisor = str(destinator[0])
                        print nodo_emisor
                        nodo_receptor = str(datonodo)
                        print nodo_receptor
                        t = datetime.datetime.now()
                        fecha = str(t.day) + "/" + str(t.month) + "/" + str(t.year)
                        print fecha
                        insert_RSSI(str(fecha), nodo_emisor, nodo_receptor, datodb)
                    except:
                        break
                else:
                    print frame
                    print 'no hay frame'
            except:
                print 'termino'
                break
        print 'paso al siguiente'
        """
        if discovered:
            # xbee = ZigBee(serial_port, escaped=True)
            rq_data(address, destinator[0], xbee)
            end_tx(address, destinator[0], xbee)
            # request_done = False
            completo = True
            print "esperamos a un nuevo ciclo"
        else:
            print "datos ya pedidos esperamo al siguiente ciclo"
            t = datetime.datetime.now()
            print str(t.hour) + ":" + str(t.minute) + ":" + str(t.second)
            time.sleep(0.5)
            # request_done = False
            completo = True
        """
        print "he terminado todos los datos"
    print "he salido del bucle for"


def insert_RSSI(var1, var2, var3, var4):
    try:
        con = mdb.connect('localhost', 'root', 'root', 'ardusense')
        with con:
            cur = con.cursor()
            # update ardusense.endpoints set endp_address='0013A200:408BC81E' where endp_id='endp_1'
            # print("SELECT endp_id,endp_address FROM ardusense.endpoints;")
            print "guardando datos"
            # cursor.execute("INSERT INTO ardusense.antenas (fecha, emisor, receptor, intensidad) VALUES (?, ?, ?, ?);",'cassete' ,'moto' ,'rosa', 'malva')
            # cur.execute("INSERT INTO `ardusense`.`antenas` (`fecha`, `emisor`, `receptor`, `intensidad`) VALUES (?, ?, ?, ?);"),'LIBRO', 'CASCO', 'naranja', 'morado')
            cur.execute("INSERT INTO `ardusense`.`antenas` (`fecha`, `emisor`, `receptor`, `intensidad`) "
                        "VALUES ( '" + str(var1) + "', '" + str(var2) + "', '" + str(var3) + "', '" + str(
                ord(var4)) + "' );")
            # cur.execute("INSERT INTO `ardusense`.`antenas` (`fecha`, `emisor`, `receptor`, `intensidad`) VALUES ('LIBRO', 'CASCO', 'naranja', 'morado');")
            # cur.execute("SELECT * FROM ardusense.antenas order by id asc;")
            # cur.execute("SELECT endp_id,endp_address FROM ardusense.endpoints order by id asc;")
            # endp_params = cur.fetchall()
            # print endp_params
            # return endp_params
            print "datos guardados"
    except con.Error as err:
        if (output == 1):
            print("Something went wrong, working with  database: {}".format(err))
        return endp_params
        serial_port.close()
        sys.exit(0)
    except Exception as e:

        print e


def main():
    global output
    try:
        # GPIO.setmode(GPIO.BCM)
        # GPIO.setup(22,GPIO.OUT)
        # GPIO.output(22,1)
        # GPIO.setup(6,GPIO.OUT)
        # GPIO.output(6,1)
        # GPIO.setup(13,GPIO.OUT)
        # GPIO.output(13,0)
        # GPIO.setup(19,GPIO.OUT)
        # GPIO.output(19,1)
        # GPIO.setup(26,GPIO.OUT)
        # GPIO.output(26,0)
        # serial_port = serial.Serial('/dev/ttyAMA0', 9600)
        # import datetime
        serial_port = serial.Serial('/dev/ttyUSB0', 9600, timeout=2)
        # serial_port.inWaiting()
        xbee = ZigBee(serial_port, escaped=True)
        data_available = 0

        # con = mdb.connect('localhost', 'drl', 'hrl2015', 'ardusense')#######Connect to Data Base MySQL#######Connect to Data Base MySQL

        timeout = True

        # t = datetime.datetime.now()
        # print t.minute
        output = 0
        if len(sys.argv) != 2:
            print "Error Usage:  SAN_Manager_v8.py 0/1"
            sys.exit()
        output = int(sys.argv[1])
        if output == 1:
            print "Display option activated"
        serveforever(xbee)


    except KeyboardInterrupt:
        # sock_in.close()
        # t1.exit()
        # t2.exit()
        print 'Closing server'
        print("Ctrl+c pressed, exit")
        xbee.halt()
        serial_port.close()
        GPIO.output(6, 0)
        # GPIO.setup(19,GPIO.OUT)
        # GPIO.output(19,1)
        # GPIO.setup(26,GPIO.OUT)
        GPIO.output(19, 0)
        sys.exit(0)


if __name__ == "__main__":
    main()

