import './crosshair.css';
import { arrayRange } from "../../utils";
import { memo } from 'react';

export const Crosshair: React.FC = memo(() => {
    // TODO: should we determine tick # and spacing based on screen size?
    const nVerticalTicks = 8;
    const verticalTicks = arrayRange(nVerticalTicks);
    const nHorizontalTicks = 12;
    const horizontalTicks = arrayRange(nHorizontalTicks);
    return (
        <div className="crosshair-container">
            <div className="vertical-axis">
                <div>
                    {verticalTicks.map((value: number) => (
                        <div key={`top-tick-${value}`} className="tick"/>
                    ))}
                </div>
                <div>
                    {verticalTicks.map((value: number) => (
                        <div key={`bottom-tick-${value}`} className="tick"/>
                    ))}
                </div>
            </div>
            <div className="horizontal-axis">
                <div>
                {horizontalTicks.map((value: number) => (
                    <div key={`left-tick-${value}`} className="tick"/>
                ))}
                </div>
                <div>
                    {horizontalTicks.map((value: number) => (
                        <div key={`right-tick-${value}`} className="tick"/>
                    ))}
                </div>
            </div>
        </div>
    )
})