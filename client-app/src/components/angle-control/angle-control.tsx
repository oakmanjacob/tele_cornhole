import './angle-control.css';

import { ReactComponent as AngleControlBG } from './angle-control-bg.svg';
import { ReactComponent as AnglePointer } from './angle-pointer.svg';
import { CSSProperties } from 'react';

interface IAngleControlProps {
    theta: number;
}

export const AngleControl: React.FC<IAngleControlProps> = ({ theta }) => {
    const anglePointerStyle: CSSProperties = {
        transformOrigin: '50% calc(100% - 8px)',
        transform: `rotate(${theta}deg)`
    }
    return (
        <div className="angle-control-container">
            <AngleControlBG />
            <AnglePointer style={anglePointerStyle} />
        </div>
    )
}