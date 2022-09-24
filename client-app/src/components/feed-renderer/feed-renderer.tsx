import { connect, io } from 'socket.io-client';
import { useRef } from "react";

const webcamURL = 'http://10.39.88.101:8735';

export const FeedRenderer: React.FC = () => {
    // make socket connection

    const imgRef = useRef<HTMLImageElement>(null);

    const socketRef = useRef((() => {
        console.log('connecting, should only print once')
        return connect(webcamURL)
    })());

    // connect(webcamURL)

    // socketRef.current.on('connect', () => {
    //     const engine = socketRef.current.io.engine;
    //     console.log(engine.transport.name); // in most cases, prints "polling"

    //     engine.on("data", (data) => {
    //         // called for each packet received
    //         console.log(data)
    //     });
    // })

    socketRef.current.onAny(() => {
        console.log("RECEIVED SOMETHING")
    })

    socketRef.current.on('connect', () => {
        socketRef.current.emit('connected', 'Hello from client')
    })

    // socketRef.current.emit('foo', 'hello')

    socketRef.current.on('data', (data: any) => {
        console.log(data.toString())
    })

    // socketRef.current.connect();

    // const paintFrame = () => {
    //     if (!imgRef.current) {
    //         return;
    //     }
    //     imgRef.current.src = '';
    // }

    // First effect to listen to socket and parse data

    // Second effect to "paint"

    return <>
        <img ref={imgRef} alt="camera feed" />
    </>
}