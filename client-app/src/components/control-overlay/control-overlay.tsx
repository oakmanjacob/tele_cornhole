import { useState, useCallback, useEffect } from "react";
import { DepVarPipe, useNormalizedOscillator } from "../../services/control-keyboard-listener/use-normalized-oscillator";
import { AngleControl } from "../angle-control/angle-control";
import { LinearPowerControl } from "../linear-power-control/linear-power-control";
import { linearPowerControlStyle, pitchControlStyle, yawControlStyles } from "./control-overlay.styles";

const linearToPercent: DepVarPipe = (rawX) => (rawX + 1) * 50;
const toDeg: DepVarPipe = (rawX) => rawX * 90;

enum ControlEmbedding {
    Pitch = 'Pitch',
    Power = 'Power',
    Yaw = 'Yaw',
    None = 'None'
}

const degreesOfFreedom = [
    ControlEmbedding.Yaw,
    ControlEmbedding.Pitch,
    ControlEmbedding.Power
]

export const ControlOverlay: React.FC = () => {
    const [iDOF, setIDOF] = useState(0);

    const { x: yaw } = useNormalizedOscillator(degreesOfFreedom[iDOF] === ControlEmbedding.Yaw, { pipe: toDeg, frequency: 0.0005});
    const { x: pitch } = useNormalizedOscillator(degreesOfFreedom[iDOF] === ControlEmbedding.Pitch, { pipe: toDeg, frequency: 0.0005});
    const { x: power } = useNormalizedOscillator(degreesOfFreedom[iDOF] === ControlEmbedding.Power, { pipe: linearToPercent, frequency: 0.0005});
    
    const spacebarHandler = useCallback((keyboardEvent: KeyboardEvent) => {
        switch (keyboardEvent.code) {
            case 'Space':
                console.log('Space');
                if (iDOF === degreesOfFreedom.length - 1) {
                    console.log(`Send coords to server: (${yaw}, ${pitch}, ${power})`)
                } else if (iDOF >= degreesOfFreedom.length) {
                    return;
                }
                setIDOF(iDOF + 1);
                break;
    
        }
    }, [iDOF, pitch, power, yaw])

    // Listener
    useEffect(() => {
        window.addEventListener('keydown', spacebarHandler);
        return () => {
            window.removeEventListener('keydown', spacebarHandler);
        }
    }, [spacebarHandler])

    return (
        <div style={{zIndex: 2}}>
            <div style={yawControlStyles}><AngleControl theta={yaw}/></div>
            {iDOF >= 1 && <div style={pitchControlStyle}><AngleControl theta={pitch}/></div>}
            {iDOF >= 2 && <div style={linearPowerControlStyle}><LinearPowerControl percentPower={power} /></div>}
            {iDOF >= 3 && <button onClick={() => setIDOF(0)} style={{ right: '24px', top: '24px', position: 'absolute', zIndex: '3'}}>Reset</button>}
        </div>
    )
}