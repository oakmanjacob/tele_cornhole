import { useState, useCallback, useEffect } from "react";
import { DepVarPipe, useNormalizedOscillator } from "../../services/control-keyboard-listener/use-normalized-oscillator";
import { AxisControl, AxisControlOrientation } from "../axis-control/axis-control";
import { Crosshair } from "../crosshair/crosshair";
import { PowerControl } from "../power-control/power-control";

const toPercent: DepVarPipe = (rawX) => (rawX + 1) * 50;

enum ControlEmbedding {
    Horizontal = 'Horizontal',
    Vertical = 'Vertical',
    Power = 'Power',
    None = 'None'
}

const degreesOfFreedom = [
    ControlEmbedding.Horizontal,
    ControlEmbedding.Vertical,
    ControlEmbedding.Power
]

export const ControlOverlay: React.FC = () => {
    const [iDOF, setIDOF] = useState(0);

    const { x: offsetX } = useNormalizedOscillator(degreesOfFreedom[iDOF] === ControlEmbedding.Horizontal, { pipe: toPercent, frequency: 0.0005});
    const { x: offsetY } = useNormalizedOscillator(degreesOfFreedom[iDOF] === ControlEmbedding.Vertical, { pipe: toPercent, frequency: 0.0005});
    const { x: offsetR } = useNormalizedOscillator(degreesOfFreedom[iDOF] === ControlEmbedding.Power, { frequency: 0.0001});
    
    const spacebarHandler = useCallback((keyboardEvent: KeyboardEvent) => {
        switch (keyboardEvent.code) {
            case 'Space':
                console.log('Space');
                if (iDOF === degreesOfFreedom.length - 1) {
                    console.log(`Send coords to server: (${offsetX}, ${offsetY}, ${offsetR})`)
                } else if (iDOF >= degreesOfFreedom.length) {
                    return;
                }
                setIDOF(iDOF + 1);
                break;
    
        }
    }, [iDOF, offsetR, offsetX, offsetY])

    // Listener
    useEffect(() => {
        window.addEventListener('keydown', spacebarHandler);
        return () => {
            window.removeEventListener('keydown', spacebarHandler);
        }
    }, [spacebarHandler])

    return (
        <div style={{zIndex: 2}}>
            <Crosshair />
            <AxisControl offset={offsetX} orientation={AxisControlOrientation.Vertical} />
            {iDOF >= 1 && <AxisControl offset={offsetY} orientation={AxisControlOrientation.Horizontal} />}
            {iDOF >= 2 && <PowerControl x={offsetX} y={offsetY} r={offsetR} />}
            {iDOF >= 3 && <button onClick={() => setIDOF(0)} style={{ right: '24px', top: '24px', position: 'absolute', zIndex: '3'}}>Reset</button>}
        </div>
    )
}