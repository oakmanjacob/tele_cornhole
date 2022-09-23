import { memo } from 'react';
import './axis-control.css'

export enum AxisControlOrientation {
    Horizontal = 'Horizontal',
    Vertical = 'Vertical'
}

interface IAxisControlProps {
    // For now, offset will be a percentage
    offset: number;
    orientation: AxisControlOrientation
}

export const AxisControl: React.FC<IAxisControlProps> = memo(({ offset, orientation }) => {
    const positionStyle = (() => {
        switch (orientation) {
            case AxisControlOrientation.Vertical:
                return {
                    left: `${offset}%`
                }
            case AxisControlOrientation.Horizontal:
                return {
                    top: `${offset}%`
                }
            default: 
                return {}
        }
    })()
    return <div className='axis-control' data-orientation={orientation} style={positionStyle}></div>
})