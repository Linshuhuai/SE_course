#include <stdint.h>
#include <inttypes.h>
#include <rte_eal.h>
#include <rte_ethdev.h>
#include <rte_cycles.h>
#include <rte_lcore.h>
#include <rte_mbuf.h>
#include <rte_ether.h>
#include <rte_ip.h>
#include <rte_udp.h>

#define RX_RING_SIZE 1024
#define TX_RING_SIZE 1024

#define NUM_MBUFS 8191
#define MBUF_CACHE_SIZE 250
#define BURST_SIZE 32

static const struct rte_eth_conf port_conf_default = {
	.rxmode = { .max_rx_pkt_len = ETHER_MAX_LEN }
};

/* basicfwd.c: Basic DPDK skeleton forwarding example. */

/*
 * Initializes a given port using global settings and with the RX buffers
 * coming from the mbuf_pool passed as a parameter.
 */
static inline int port_init(uint16_t port, struct rte_mempool *mbuf_pool) {
	struct rte_eth_conf port_conf = port_conf_default;
	const uint16_t rx_rings = 1, tx_rings = 1;
	uint16_t nb_rxd = RX_RING_SIZE;
	uint16_t nb_txd = TX_RING_SIZE;
	int retval;
	uint16_t q;
	struct rte_eth_txconf txconf;

	/* Configure the Ethernet device. */
	retval = rte_eth_dev_configure(port, rx_rings, tx_rings, &port_conf);
	if (retval != 0)
		return retval;

	/* Allocate and set up 1 RX queue per Ethernet port. */
	for (q = 0; q < rx_rings; q++) {
		retval = rte_eth_rx_queue_setup(port, q, nb_rxd,
				rte_eth_dev_socket_id(port), NULL, mbuf_pool);
		if (retval < 0)
			return retval;
	}

	/* Allocate and set up 1 TX queue per Ethernet port. */
	for (q = 0; q < tx_rings; q++) {
		retval = rte_eth_tx_queue_setup(port, q, nb_txd,
				rte_eth_dev_socket_id(port), &txconf);
		if (retval < 0)
			return retval;
	}

	/* Start the Ethernet port. */
	retval = rte_eth_dev_start(port);
	if (retval < 0)
		return retval;

	return 0;
}

/*
 * The lcore main. This is the main thread that does the work, reading from
 * an input port and writing to an output port.
 */
static __rte_noreturn void lcore_main(struct rte_mempool *mbuf_pool) {
	uint16_t port;
	struct rte_mbuf * bufs[BURST_SIZE];
	int i;
	for(i = 0; i < BURST_SIZE; i++) {
		bufs[i] = rte_pktmbuf_alloc(mbuf_pool);

		/* Initialization */
		struct rte_ether_hdr* eth_hdr = rte_pktmbuf_mtod(bufs[i], struct rte_ether_hdr *);
		struct rte_ipv4_hdr* ip_hdr = (struct rte_ipv4_hdr* )(rte_pktmbuf_mtod(bufs[i], char *) + sizeof(struct rte_ether_hdr));
		struct rte_udp_hdr* udp_hdr = (struct rte_udp_hdr* )(rte_pktmbuf_mtod(bufs[i], char *) + sizeof(struct rte_ether_hdr) + sizeof(struct rte_ipv4_hdr));
		char* data = (char* )(rte_pktmbuf_mtod(bufs[i], char *) + sizeof(struct rte_ether_hdr) + sizeof(struct rte_ipv4_hdr) + sizeof(struct rte_udp_hdr));
		
		/* Ethernet header */
		struct rte_ether_addr s_addr = {{0x00,0x0C,0x29,0xA4,04F,0xC5}};
		struct rte_ether_addr d_addr =  {{0x00,0x50,0x56,0xC0,0x00,0x02}};
		eth_hdr->d_addr = d_addr;
		eth_hdr->s_addr = s_addr;
		eth_hdr->ether_type = 0x0008;

		/* IPv4 Header */
		ip_hdr-> version_ihl = RTE_IPV4_VHL_DEF;
		ip_hdr-> type_of_service = RTE_IPV4_HDR_DSCP_MASK;
		ip_hdr-> total_length = 0x2800;
		ip_hdr-> packet_id = 0;
		ip_hdr-> fragment_offset = 0;
		ip_hdr-> time_to_live = 100;
		ip_hdr-> next_proto_id = 17;
		ip_hdr-> src_addr = 0x0a50a8c0;
		ip_hdr-> dst_addr= 0x0650a8c0;
		ip_hdr-> hdr_checksum = rte_ipv4_cksum(ip_hdr);

		/* UDP Header */
		udp_hdr-> src_port = 80;	
		udp_hdr-> dst_port = 8080;
		udp_hdr-> dgram_len = 0x1400;
		udp_hdr-> dgram_cksum = 1;

		memcpy(data, "Linshuhuai", 10);
		bufs[i]-> data_len = sizeof(struct rte_ether_hdr) + sizeof(struct rte_ipv4_hdr)+sizeof(struct rte_udp_hdr) + 10;
		bufs[i]-> pkt_len = sizeof(struct rte_ether_hdr) + sizeof(struct rte_ipv4_hdr)+sizeof(struct rte_udp_hdr) + 10;
	}

	uint16_t nb_tx = rte_eth_tx_burst(0, 0, bufs, BURST_SIZE);
	printf("Successfully send %d UDP packets\n", nb_tx);
	
	for(i = 0; i < BURST_SIZE; i++) rte_pktmbuf_free(bufs[i]);
}

/*
 * The main function, which does initialization and calls the per-lcore
 * functions.
 */
int main(int argc, char *argv[]) {
	struct rte_mempool *mbuf_pool;
	unsigned nb_ports = 1;
	uint16_t portid = 0;

	/* Initialize the Environment Abstraction Layer (EAL). */
	int ret = rte_eal_init(argc, argv);
	if (ret < 0)
		rte_exit(EXIT_FAILURE, "Error with EAL initialization\n");

	argc -= ret;
	argv += ret;

	/* Creates a new mempool in memory to hold the mbufs. */
	mbuf_pool = rte_pktmbuf_pool_create("MBUF_POOL", NUM_MBUFS * nb_ports,
		MBUF_CACHE_SIZE, 0, RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id());

	if (mbuf_pool == NULL)
		rte_exit(EXIT_FAILURE, "Cannot create mbuf pool\n");

	/* Initialize all ports. */
	// RTE_ETH_FOREACH_DEV(portid)
		if (port_init(portid, mbuf_pool) != 0)
			rte_exit(EXIT_FAILURE, "Cannot init port %"PRIu16 "\n",
					portid);

	/* Call lcore_main on the main core only. */
	lcore_main(mbuf_pool);

	/* clean up the EAL */
	// rte_eal_cleanup();

	return 0;
}