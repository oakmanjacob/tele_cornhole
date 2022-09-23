import { containerStyle, getBarStyle } from "./linear-power-control.styles"

interface ILinearPowerControlProps {
    percentPower: number
}

export const LinearPowerControl: React.FC<ILinearPowerControlProps> = ({ percentPower }) => {
    return (
        <div style={containerStyle}>
            <div style={getBarStyle(percentPower)} />
        </div>
    )
}