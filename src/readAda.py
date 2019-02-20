#!/usr/bin/env python
import Adafruit_BluefruitLE
import time,sys,os
import uuid
import  Adafruit_BluefruitLE.services as adaServ

def succ():
    return sys.stderr.write("Success\n")

def connectAda(ble,cooldown=1.):
    UUID_BASIC = uuid.UUID('6E400001-B5A3-F393-E0A9-E50E24DCCA9E')
    # UUID_BASIC = UUID_BASIC
    #### https://learn.adafruit.com/introducing-adafruit-ble-bluetooth-low-energy-friend/uart-service
    while True:
        ble.clear_cached_data()
        adapter = ble.get_default_adapter()
#         adapter.power_on()
#         try:
#             adapter.start_scan(1.)
#         finally:
#             # Make sure scanning is stopped before exiting.
#             adapter.stop_scan()        
#         try:
#             adapter.start_scan()
#             # Search for the first UART device found (will time out after 60 seconds
#             # but you can specify an optional timeout_sec parameter to change it).
#             device = ble.find_device(service_uuids=[UART_SERVICE_UUID])
#             if device is None:
#                 raise RuntimeError('Failed to find UART device!')
#                 continue 
#         finally:
#             # Make sure scanning is stopped before exiting.
#             adapter.stop_scan()


        
#         adapter.stop_scan()
#         adapter.start_scan()
        sys.stderr.write("[MSG] Finding Device for Service...%s\n"%UUID_BASIC)
        dev = ble.find_device(service_uuids=[UUID_BASIC],timeout_sec=3)
        if dev is None:
            sys.stderr.write("...Retry\n")
            time.sleep(cooldown)
#             adapter.stop_scan()
            continue 
#         adapter.stop_scan()
        succ()
        # assert dev is not None,'cannot find device with service uuid: %s'%UUID_BASIC

#         time.sleep(cooldown)
#         try:
#             adaServ.UART.disconnect_devices(timeout_sec = 5)
#         except RuntimeError as e:
#             sys.stderr.write("[IGNORE]%s\n"%e)
    
        sys.stderr.write("[MSG] Connecting to Device:%s\n"%UUID_BASIC)
        try:
            dev.connect(timeout_sec = 5)
        except RuntimeError as e:
            sys.stderr.write("[IGNORE]%s\n"%e)
            
        if not dev.is_connected:
            sys.stderr.write("...Retry\n")
            time.sleep(cooldown)
            continue 
        succ()
            
        sys.stderr.write("[MSG] Finding Service...%s\n"%UUID_BASIC)
        serv = dev.find_service(UUID_BASIC)
        if serv is None:
            sys.stderr.write("...Retry\n")
            time.sleep(cooldown)
            continue 
        succ()
        
        chars = serv.list_characteristics()

        uuid2channel = {'6e400002':'TX','6e400003':'RX'}
        out = {'dev':dev}
        lst = serv.list_characteristics()
        for x in lst:
            uuidhex= x.uuid.get_hex()
            channel = uuid2channel.get(uuidhex[:8],None)
            assert channel is not None, 'cannot identify uuid:dict:%s\nquery:%s'%(uuid2channel,uuidhex)
            out[channel] = x        
        chars = util_obj(**out)
        assert hasattr(chars,'RX'),'cannot find RX characteristic'
        assert hasattr(chars,'TX'),'cannot find RX characteristic'
        return chars

def main(ble, dt = 0.5):
    chars = connectAda(ble)
    try:
        sys.stderr.write('[MSG] Starting recording\n')
        header = 'time,blue,green\n'
        stdout.write(header)
        while True:
            dval = chars.RX.read_value()
            s  = ''.join(map(str,dval))
            s = str(time.time())+','+s
            stdout.write(s)
#                 s  = s.strip().split(',')
            time.sleep(dt)                
#         except Exception as e:
    except KeyboardInterrupt as e:
        raise e
    finally:
        try:
            sys.stderr.write('[MSG] Disconnecting...\n')
            chars.dev.disconnect(timeout_sec=3)
        except RuntimeError as e:
            sys.stderr.write('[IGNORE]%s\n'%e)

class util_obj(object):
    def __init__(self,**kwargs):
        self.set_attr(**kwargs)
        pass
    
    def reset(self,method):
        mthd = getattr(self,method)
        if isinstance(mthd, functools.partial):
            setattr(self,method,mthd.func)
        else:
            print "[WARN]:Trying to reset a native method"
        pass

    def partial(self,attrN,**param):
        attr = getattr(self,attrN)
        newattr = functools.partial(attr,**param)
        setattr(self,attrN,newattr)
        pass
    def set_attr(self,**param):
        for k,v in param.items():
            setattr(self, k, v)
        return self
    
    def __getitem__(self,k):
        return self.__dict__[k]
    def __setitem__(self,k,v):
        self.__dict__[k] = v

        
if __name__ =='__main__':
    stdout = os.fdopen(sys.stdout.fileno(), 'w', 0)
    
    ble = Adafruit_BluefruitLE.get_provider()

    sys.stderr.write("[MSG] Initializing...")
    ble.initialize()
    succ()
    while True:
        try:
            main(ble)
        except KeyboardInterrupt:
            sys.stderr.write('Interrupted\n')
            sys.exit(0)
#             try:
#                 sys.exit(0)
#             except SystemExit:
#                 os._exit(0)        
#                 main()
#     ble.run_mainloop_with(main)