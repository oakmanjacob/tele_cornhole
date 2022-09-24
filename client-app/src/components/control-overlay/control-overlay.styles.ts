import { CSSProperties } from "react";

export const controlOverlayStyles: CSSProperties = {
    position: 'relative',
    height: '720px',
    width: '1280px',
}

export const controlOverlayContainerStyles: CSSProperties = {
    position: 'relative',
    height: '720px',
    width: '1280px',
    zIndex: '2',
    display: 'grid',
    gridTemplateColumns: '1fr 1fr 1fr',
    gridTemplateRows: '1fr 1fr 1fr',
    placeItems: 'center'
}

export const yawControlStyles: CSSProperties = {
    // position: 'absolute',
    // bottom: '36px',
    // left: '50%',
    // transform: 'translateX(-50%)',
    gridRow: '3 / 4',
    gridColumn: '2 / 3',
    width: '256px',
    // width: '512px',
    // height: '512px',
}

export const yawLabelStyles: CSSProperties = {

}

export const pitchControlStyle: CSSProperties = {
    // position: 'absolute',
    // bottom: '36px',
    // left: '36px',
    transform: 'rotate(45deg)',
    gridRow: '3 / 4',
    gridColumn: '1 / 2',
    width: '256px',
    // height: '512px',
}

export const linearPowerControlStyle: CSSProperties = {
    position: 'absolute',
    bottom: '36px',
    right: '36px',
}