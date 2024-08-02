import React from 'react';
import { Line } from 'react-chartjs-2';
import { Chart as ChartJS, LineElement, CategoryScale, LinearScale, PointElement } from 'chart.js';

// Register Chart.js components
ChartJS.register(LineElement, CategoryScale, LinearScale, PointElement);

const FlightPaths = ({ flights }) => {
  const adjustPaths = (flights) => {
    const adjustedFlights = flights.map(flight => flight.slice());

    // Function to check if two line segments intersect
    const doIntersect = ([x1, y1], [x2, y2], [x3, y3], [x4, y4]) => {
      const orientation = (p, q, r) => {
        const val = (q[1] - p[1]) * (r[0] - q[0]) - (q[0] - p[0]) * (r[1] - q[1]);
        if (val === 0) return 0; // collinear
        return val > 0 ? 1 : 2; // clock or counterclock wise
      };

      const o1 = orientation([x1, y1], [x2, y2], [x3, y3]);
      const o2 = orientation([x1, y1], [x2, y2], [x4, y4]);
      const o3 = orientation([x3, y3], [x4, y4], [x1, y1]);
      const o4 = orientation([x3, y3], [x4, y4], [x2, y2]);

      return (o1 !== o2 && o3 !== o4);
    };

    // Adjust paths to avoid intersections by adding intermediate points
    const adjustPath = (flight1, flight2) => {
      for (let i = 1; i < flight1.length; i++) {
        for (let j = 1; j < flight2.length; j++) {
          if (doIntersect(flight1[i - 1], flight1[i], flight2[j - 1], flight2[j])) {
            // Add intermediate points to avoid intersection
            const midPoint1 = [
              (flight1[i - 1][0] + flight1[i][0]) / 2,
              (flight1[i - 1][1] + flight1[i][1]) / 2
            ];
            const midPoint2 = [
              (flight2[j - 1][0] + flight2[j][0]) / 2,
              (flight2[j - 1][1] + flight2[j][1]) / 2
            ];

            flight1.splice(i, 0, midPoint1);
            flight2.splice(j, 0, midPoint2);

            // Re-check the adjusted segments for intersections
            return adjustPath(flight1, flight2);
          }
        }
      }
    };

    for (let i = 0; i < adjustedFlights.length; i++) {
      for (let j = i + 1; j < adjustedFlights.length; j++) {
        adjustPath(adjustedFlights[i], adjustedFlights[j]);
      }
    }

    return adjustedFlights;
  };

  const adjustedFlights = adjustPaths(flights);

  // Prepare data for Chart.js
  const data = {
    datasets: adjustedFlights.map((flight, index) => ({
      label: `Flight ${index + 1}`,
      data: flight.map(([x, y]) => ({ x, y })),
      borderColor: ['#FF5733', '#33FF57', '#3357FF'][index % 3], // Different colors for different flights
      backgroundColor: 'rgba(0,0,0,0)',
      borderWidth: 2,
      fill: false,
      pointRadius: 5,
    })),
  };

  // Chart.js options
  const options = {
    scales: {
      x: {
        type: 'linear',
        position: 'bottom',
        title: {
          display: true,
          text: 'X Coordinate',
        },
      },
      y: {
        title: {
          display: true,
          text: 'Y Coordinate',
        },
      },
    },
    plugins: {
      legend: {
        display: true,
      },
    },
  };

  return (
    <div>
      <h2>Flight Paths</h2>
      <Line data={data} options={options} />
    </div>
  );
};

// Example usage
const flights = [
  [[1, 1], [2, 2], [3, 3]], // Flight 1
  [[1, 1], [2, 4], [3, 2]], // Flight 2
  [[1, 1], [4, 2], [3, 4]], // Flight 3
];

const App = () => (
  <div>
    <FlightPaths flights={flights} />
  </div>
);

export default App;
