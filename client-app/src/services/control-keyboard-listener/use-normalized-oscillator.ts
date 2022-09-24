import { useCallback, useEffect, useRef, useState } from "react";

const MS_PER_S = 1000;
const ONE_HERTZ = 1 / MS_PER_S;

export type DepVarPipe = (rawX: number) => number;

type UseNormalizedOscillatorOptions = {
    pipe?: DepVarPipe;
    frequency?: number;
}

const defaultOptions: UseNormalizedOscillatorOptions = {
    pipe: (x) => x,
    frequency: ONE_HERTZ
} as const

export const useNormalizedOscillator = (active: boolean, options: UseNormalizedOscillatorOptions = defaultOptions) => {

    const period = 1 / (options?.frequency || ONE_HERTZ)

    const [x, setX] = useState<number>(0);
    const start = useRef<DOMHighResTimeStamp>(-1);

    const requestAnimRef = useRef<number>(-1);

    const updateX: FrameRequestCallback = useCallback((timeStamp: DOMHighResTimeStamp) => {
        if(!active) {
            cancelAnimationFrame(requestAnimRef.current)
            return;
        }
        // init
        if (start.current === -1) {
            start.current = timeStamp;
        }

        // main
        const elapsedTime = timeStamp - start.current;
        const t = 2 * Math.PI * elapsedTime / period;

        // temp use sine to transform easily... hackily
        setX(Math.sin(t));
        requestAnimRef.current = requestAnimationFrame(updateX);
    }, [active, period])

    useEffect(() => {
        if (!active) {
            return;
        }
        requestAnimRef.current = requestAnimationFrame(updateX);
        return () => cancelAnimationFrame(requestAnimRef.current);
    }, [active, updateX])

    return { x: options.pipe ? options.pipe(x) : x }
}