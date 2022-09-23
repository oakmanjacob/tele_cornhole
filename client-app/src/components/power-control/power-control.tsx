import './power-control.css';

interface IPowerControlProps {
    x?: number;
    y?: number;
    r: number;
}

export const PowerControl: React.FC<IPowerControlProps> = ({x = 50, y = 50, r}) => {
    const containerPositionStyle = {
        left: `${x}%`,
        top: `${y}%`,
        transform: 'translate(-50%, -50%)'
    }
    const circleRadiusStyle = {
        transform: `scale(${r})`
    }
    
    return (
        <div className="power-control-container" style={containerPositionStyle}>
            <div className="power-control-circle" style={circleRadiusStyle}/>
        </div>
    )
}