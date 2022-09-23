import { CSSProperties } from "react";

export const controlOverlayStyles: CSSProperties = {
    position: 'relative',
    height: '720px',
    width: '1280px',
}

export const yawControlStyles: CSSProperties = {
    position: 'absolute',
    bottom: '36px',
    left: '50%',
    transform: 'translateX(-50%)',
}

export const pitchControlStyle: CSSProperties = {
    position: 'absolute',
    bottom: '36px',
    left: '36px',
    transform: 'rotate(90deg)'
}

export const linearPowerControlStyle: CSSProperties = {
    position: 'absolute',
    bottom: '36px',
    right: '36px',
}