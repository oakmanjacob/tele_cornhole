import { CSSProperties } from "react";

export const containerStyle: CSSProperties = {
    position: 'relative',
    height: '256px',
    width: '32px',
    border: '2px solid var(--dcc-linear-power-color)',
    display: 'flex',
    flexDirection: 'column',
    justifyContent: 'flex-end',
}

export const getBarStyle = (percentPower: number): CSSProperties => ({
    width: '100%',
    height: `${percentPower}%`,
    background: 'var(--dcc-linear-power-color)'
})