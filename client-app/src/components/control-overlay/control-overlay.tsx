import axios from "axios";
import { useState, useCallback, useEffect } from "react";
import { DepVarPipe, useNormalizedOscillator } from "../../services/control-keyboard-listener/use-normalized-oscillator";
import { AngleControl } from "../angle-control/angle-control";
import { LinearPowerControl } from "../linear-power-control/linear-power-control";
import { controlOverlayContainerStyles, linearPowerControlStyle, pitchControlStyle, yawControlStyles } from "./control-overlay.styles";

const linearToPercent: DepVarPipe = (rawX) => (rawX + 1) * 50;

// range y: -40 - 50
// range x: -48 + 48

const boundY = (deg: number) => {
    return Math.max(-40, Math.min(50, deg))
}

const boundX = (deg: number) => {
    return Math.max(-48, Math.min(48, deg))
}

// oscillator
const toDeg: DepVarPipe = (rawX) => rawX * 15;

enum ControlEmbedding {
    Pitch = 'Pitch',
    Power = 'Power',
    Yaw = 'Yaw',
    None = 'None'
}

const degreesOfFreedom = [
    ControlEmbedding.Yaw,
    ControlEmbedding.None,
    ControlEmbedding.Pitch,
    ControlEmbedding.None,
    ControlEmbedding.Power
]

const baseURL = 'http://10.39.90.14:5000';

export const ControlOverlay: React.FC = () => {
    const [iDOF, setIDOF] = useState(0);

    const { x: yaw } = useNormalizedOscillator(degreesOfFreedom[iDOF] === ControlEmbedding.Yaw, { pipe: toDeg, frequency: 0.0005});
    const { x: pitch } = useNormalizedOscillator(degreesOfFreedom[iDOF] === ControlEmbedding.Pitch, { pipe: toDeg, frequency: 0.0005});
    const { x: power } = useNormalizedOscillator(degreesOfFreedom[iDOF] === ControlEmbedding.Power, { pipe: linearToPercent, frequency: 0.0005});
    
    const spacebarHandler = useCallback((keyboardEvent: KeyboardEvent) => {
        switch (keyboardEvent.code) {
            case 'Space':
                // Send appropriate request, set to none while waiting, set to next
                switch (degreesOfFreedom[iDOF]) {
                    case ControlEmbedding.Yaw:
                        setIDOF(iDOF + 1)
                        axios.get(`${baseURL}/xrot/${boundX(Math.round(yaw))}`, { timeout: 60000 }).then((value) => {
                            console.log(value);
                            if(value.status === 200) {
                                setIDOF(iDOF + 2)
                            }
                        })
                        break;
                    case ControlEmbedding.Pitch:
                        setIDOF(iDOF + 1)
                        axios.get(`${baseURL}/yrot/${boundY(Math.round(-pitch + 30))}`, { timeout: 60000 }).then((value) => {
                            console.log(value);
                            if(value.status === 200) {
                                setIDOF(iDOF + 2)
                            }
                        })
                        break;
                    case ControlEmbedding.Power:
                        setIDOF(iDOF + 1)
                        axios.get(`${baseURL}/fire`, { timeout: 60000 }).then((value) => {
                            console.log(value);
                            if(value.status === 200) {
                                setIDOF(iDOF + 2)
                            }
                        })
                        break;
                    case ControlEmbedding.None:
                    default:
                        break;
                }
                break;
    
        }
    }, [iDOF, pitch, yaw])

    // Listener
    useEffect(() => {
        window.addEventListener('keydown', spacebarHandler);
        return () => {
            window.removeEventListener('keydown', spacebarHandler);
        }
    }, [spacebarHandler])

    return (
        <div style={controlOverlayContainerStyles}>
            <div style={yawControlStyles}><AngleControl theta={yaw}/></div>
            {iDOF >= 2 && <>
                <div style={pitchControlStyle}><AngleControl theta={pitch}/></div>
            </>}
            {iDOF >= 4 && <div style={linearPowerControlStyle}><LinearPowerControl percentPower={power} /></div>}
            {iDOF >= 5 && <button onClick={() => setIDOF(0)} style={{ right: '24px', top: '24px', position: 'absolute', zIndex: '3'}}>Reset</button>}
        </div>
    )
}